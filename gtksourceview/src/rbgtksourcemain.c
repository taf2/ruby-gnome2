/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtksourcemain.c -

  $Author $
  $Date: 2004/08/05 18:13:49 $

  Copyright (C) 2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003 Geoff Youngs, based on gtktextview.c by Masao Mutoh
************************************************/
#include "rbgtksourcemain.h"

extern void Init_gtk_sourceview (void);
extern void Init_gtk_sourcetag (void);
extern void Init_gtk_sourcetagstyle (void);
extern void Init_gtk_sourcetagtable (void);
extern void Init_gtk_sourcelanguage (void);
extern void Init_gtk_sourcelanguagesmanager (void);
extern void Init_gtk_sourcemarker (void);
extern void Init_gtk_sourcebuffer (void);
extern void Init_gtk_sourceiter (void);

void
Init_gtksourceview (void)
{
	Init_gtk_sourceview ();
	Init_gtk_sourcetag ();
	Init_gtk_sourcetagstyle ();
	Init_gtk_sourcetagtable ();
	Init_gtk_sourcelanguage ();
	Init_gtk_sourcelanguagesmanager ();
	Init_gtk_sourcemarker ();
	Init_gtk_sourcebuffer ();
	Init_gtk_sourceiter ();
}
