void
grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = m->wh / (rows ? rows : 1);
	cw = m->ww / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx + (i / rows) * cw;
		cy = m->wy + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - cw * cols : 0;
		resize(c, cx, cy, cw - 2 * c->bw + aw, ch - 2 * c->bw + ah, False);
		i++;
	}
}

void
magic(Monitor *m) {
  unsigned int i, n, h, w, x, y,mw, columns, size;
  Client *c;

  for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

  // Set up monitor size based heuristics.
  if (m->ww > 2000) {
    size = 1;
    columns = 2;
  } else {
    size = 0;
    columns = 1;
  }

  if(n == 0)
    return;
  if(n > columns) {
    mw = (m->ww / (columns + 1)) * columns;
  }
  else {
    mw = m->ww;
  }

  // Handle the special case of a single client.
  if (n == 1) {
    c = nexttiled(m->clients);
    if (size == 1) {
      w = (m->ww / 5) * 3;
      h = (m->wh / 8) * 7;
    } else {
      w = m->ww;
      h = m->wh;
    }
    resize(c,
           m->wx + (m->ww - w)/2,
           m->wy + (m->wh - h)/2,
           w - (2*c->bw),
           h - (2*c->bw),
           False);
    return;
  }

  for(i = x = y = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
    if(i < columns) {
      w = mw / columns;
      resize(c, x + m->wx, m->wy, w - (2*c->bw), m->wh - (2*c->bw), False);
      x += WIDTH(c);
    }
    else {
      // For the first client outside master, reserve half the monitor height,
      // except if it's the only client outside master.
      if (y == 0 && (n - columns) != 1) {
        h = (m->wh / 2);
      } else {
        h = (m->wh - y) / (n - i);
      }
      resize(c, x + m->wx, m->wy + y, m->ww - x  - (2*c->bw), h - (2*c->bw), False);
      y += HEIGHT(c);
    }
  }
}
