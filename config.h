/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 7;       /* gap pixel between windows */
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
static const int showtab            = showtab_never; /* Default tab bar show mode */
static const int toptab             = True;    /* False means bottom tab bar */

static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";
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
static const char *tags[] = { "t", "r", "e", "f", "."};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance      title  tags mask iscentered isfloating monitor */
  { NULL,                 "current",    NULL,  ~0,       1,         1,         -1 },
	{ NULL,                 "capture",    NULL,  ~0,       1,         1,         -1 },
	{ NULL,                 "agenda",     NULL,  ~0,       1,         1,         -1 },
	{ "Gnome-terminal",     NULL,         NULL,  1 << 0,   0,         0,         -1 },
	{ "st-256color",        NULL,         NULL,  ~0,       1,         1,         -1 },
  { "Google-chrome",      NULL,         NULL,  1 << 1,   0,         0,         -1 },
  { NULL,                 "emacs",      NULL,  1 << 2,   0,         0,         -1 },
	{ "copyq",              NULL,         NULL,  ~0,       1,         1,         -1 },
	{ "scrcpy",             NULL,         NULL,  1 << 4,   0,         1,         -1 },
	{ "Firefox",            NULL,         NULL,  1 << 3,   0,         0,         -1 },
	{ "vlc",                NULL,         NULL,  ~0,       0,         1,         -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
#include "fibonacci.c"
#include "horizgrid.c"

/* first entry is default */
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",     dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "[M]",      monocle },
	{ "HHH",      horizgrid },
	{ "(@)",      spiral },
	{ "[]=",      tile },
  { "><>",      NULL },    /* no layout function means floating behavior */
};

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

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
	{ MODKEY,                       XK_b,      togglebar,      {0} },
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
  { MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_m,      monomode,       {0} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ControlMask,           XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_space,  shlauncher,     {0} },
	{ MODKEY,                       XK_x,      shscreenlock,   {0} },
	{ MODKEY,                       XK_e,      sheditor,       {0} },
	{ MODKEY|ShiftMask,             XK_e,      comboview,      {.ui = 1 << 2} },
	{ MODKEY,                       XK_t,      shterm,         {0} },
	{ MODKEY|ShiftMask,             XK_t,      comboview,      {.ui = 1 << 0} },
	{ MODKEY,                       XK_f,      shbrowser,      {0} },
	{ MODKEY|ShiftMask,             XK_f,      comboview,      {.ui = 1 << 3} },
	{ MODKEY,                       XK_r,      shaltbrowser,   {0} },
	{ MODKEY|ShiftMask,             XK_r,      comboview,      {.ui = 1 << 1} },
	{ MODKEY,                       XK_c,      shclipboardmanager,    {0} },
	{ MODKEY,                       XK_p,      shpasswordmanager,     {0} },
	{ MODKEY,                       XK_a,      shapp,          {0} },
	{ MODKEY|ShiftMask,             XK_a,      shadminapp,     {0} },
	{ MODKEY,                       XK_period, combotag,       {0} },
  { MODKEY|ShiftMask,             XK_period, comboview,      {.ui = 1 << 4} },
	{ MODKEY,                       XK_1, shnumeric,          {.ui = 1} },
	{ MODKEY,                       XK_2, shnumeric,          {.ui = 2} },
	{ MODKEY,                       XK_3, shnumeric,          {.ui = 3} },
	{ MODKEY,                       XK_4, shnumeric,          {.ui = 4} },
	{ MODKEY,                       XK_5, shnumeric,          {.ui = 5} },
	{ MODKEY,                       XK_6, shnumeric,          {.ui = 6} },
	{ MODKEY,                       XK_7, shnumeric,          {.ui = 7} },
	{ MODKEY,                       XK_8, shnumeric,          {.ui = 8} },
	{ MODKEY,                       XK_9, shnumeric,          {.ui = 9} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {.v = &layouts[0]} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[7]} },
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
