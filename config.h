/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int mono               = 0;        /* 0 means mono mode */

/* System Tray */
// 0: sloppy systray follows selected monitor, >0: pin systray to monitor X
static const unsigned int systraypinning = 0;
// systray spacing
static const unsigned int systrayspacing = 4;
// 1: if pinning fails, display systray on the first monitor
// 0: display systray on the last monitor
static const Bool systraypinningfailfirst = True;
// False means no systray
static const Bool showsystray = True;

/*   Display modes of the tab bar: never shown, always shown, shown only in */
/*   monocle mode in presence of several windows.                           */
/*   Modes after showtab_nmodes are disabled                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_always, showtab_nmodes };
static const int showtab            = showtab_always; /* Default tab bar show mode */
static const int toptab             = True;    /* False means bottom tab bar */

static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_white[]       = "#ffffff";
static const char col_green[]       = "#39e6e3";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_green  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance      title  tags mask isfloating monitor */
  { NULL,                 "current",    NULL,  ~0,       1,         -1 },
	{ NULL,                 "capture",    NULL,  ~0,       1,         -1 },
	{ NULL,                 "agenda",     NULL,  ~0,       1,         -1 },
	{ "Gnome-terminal",     NULL,         NULL,  1 << 0,   0,         -1 },
	{ "st-256color",        NULL,         NULL,  ~0,       1,         -1 },
  { "Google-chrome",      NULL,         NULL,  1 << 1,   0,         -1 },
  { NULL,                 "emacs",      NULL,  1 << 2,   0,         -1 },
	{ "copyq",              NULL,         NULL,  ~0,       1,         -1 },
	{ "scrcpy",             NULL,         NULL,  1 << 4,   1,         -1 },
	{ "Firefox",            NULL,         NULL,  1 << 4,   0,         -1 },
	{ "vlc",                NULL,         NULL,  ~0,       1,         -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
/* first entry is default */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[]=",      tile },
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char scratchpadname[] = "Google-chrome-beta";
static const char *scratchpadcmd[] = { "google-chrome-beta", NULL };

/* key definitions4*/
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include "zoomswap.c"
#include "focusmaster.c"
#include "selfrestart.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
  { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
  { MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_y,      focusmaster,    {.i = -1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_apostrophe, view,       {0} },
  { MODKEY,                       XK_period, togglescratch,  {.v = scratchpadcmd} },
  { MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      monomode,       {0} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {.v = &layouts[0]} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[4]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        killclient,     {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTabBar,            0,              Button1,        focuswin,       {0} },
	{ ClkTabBar,            0,              Button2,        killclient,     {0} },
	{ ClkTabBar,            0,              Button3,        zoom,           {0} },
};
