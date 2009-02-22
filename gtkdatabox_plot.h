/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * anjuta
 * Copyright (C)  2008 <>
 * 
 * anjuta is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * anjuta is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GTK_DATABOX_PLOT_H_
#define _GTK_DATABOX_PLOT_H_

#include <glib-object.h>
#include <gtkdatabox.h>

G_BEGIN_DECLS

#define GTK_DATABOX_TYPE_PLOT             (gtk_databox_plot_get_type ())
#define GTK_DATABOX_PLOT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_DATABOX_TYPE_PLOT, GtkDataboxPlot))
#define GTK_DATABOX_PLOT_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_DATABOX_TYPE_PLOT, GtkDataboxPlotClass))
#define GTK_DATABOX_IS_PLOT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_DATABOX_TYPE_PLOT))
#define GTK_DATABOX_IS_PLOT_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_DATABOX_TYPE_PLOT))
#define GTK_DATABOX_PLOT_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_DATABOX_TYPE_PLOT, GtkDataboxPlotClass))

typedef struct _GtkDataboxPlotClass GtkDataboxPlotClass;
typedef struct _GtkDataboxPlot GtkDataboxPlot;

struct _GtkDataboxPlotClass
{
	GtkDataboxClass parent_class;
};

struct _GtkDataboxPlot
{
	GtkDatabox parent_instance;
};

GType gtk_databox_plot_get_type (void) G_GNUC_CONST;
void gtk_databox_plot_update(GtkDataboxPlot *plot, float* values);
GtkWidget *gtk_databox_plot_new (void);

G_END_DECLS

#endif /* _GTK_DATABOX_PLOT_H_ */
