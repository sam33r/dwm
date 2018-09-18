static void
focusmaster(const Arg *arg) {
  unsigned int n;
  Client *c;
  for (n = 0, c = nexttiled(selmon->clients);
      n < selmon->nmaster && c;
      c = nexttiled(c->next), n++) {
    if (n == 0)
      focus(c);
    XRaiseWindow(dpy, c->win);
  }
}
