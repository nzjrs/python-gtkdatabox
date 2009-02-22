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

#include "gtkdatabox_plot.h"

#include <gtkdatabox_points.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_bars.h>
#include <gtkdatabox_grid.h>
#include <gtkdatabox_cross_simple.h>
#include <gtkdatabox_markers.h>

#include <string.h>

typedef struct _GtkDataboxPlotPrivate GtkDataboxPlotPrivate;
struct _GtkDataboxPlotPrivate
{
	guint refresh_rate;
	guint nb_data;
	guint nb_plots;
	guint nb_points;
	guint nb_points_resize_step;
	guint time;
	gfloat *X;
	gpointer *Y;
};

#define GTK_DATABOX_PLOT_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_DATABOX_TYPE_PLOT, GtkDataboxPlotPrivate))

enum
{
	PROP_0,

	PROP_NUM_PLOTS,
	PROP_NUM_POINTS,
	PROP_NUM_POINTS_RESIZE_STEP,
	PROP_REFRESH_RATE
};

G_DEFINE_TYPE (GtkDataboxPlot, gtk_databox_plot, GTK_TYPE_DATABOX);

static const GdkColor colors[] = {
	{0, 65535, 0, 0},
	{0, 0, 65535, 0}, 
	{0, 0, 0, 65535},
	{0, 65535, 0, 65535},
	{0, 0, 0, 0}
};


const GdkColor *red = &colors[0];
const GdkColor *green = &colors[1];
const GdkColor *blue = &colors[2];
const GdkColor *pink = &colors[3];
const GdkColor *black = &colors[4];

#define NB_COLORS 4

static gboolean 
timeout_callback(gpointer user_data) {
  	GtkDatabox* databox = GTK_DATABOX (user_data);
	GtkDataboxPlotPrivate *priv = GTK_DATABOX_PLOT_PRIVATE(databox);

	if (databox == NULL || priv == NULL)
		return FALSE;

	if (priv->nb_data > 1 && priv->refresh_rate)
  		gtk_databox_auto_rescale (databox, 0.);

  	return TRUE;
}

static void
gtk_databox_plot_init (GtkDataboxPlot *object)
{
	GtkDataboxPlotPrivate *priv = GTK_DATABOX_PLOT_PRIVATE(object);
	priv->nb_data = 0;
	priv->time = 0;
	priv->X = NULL;
	priv->Y = NULL;
}

static GObject *
gtk_databox_plot_constructor (GType gtype, guint n_properties, GObjectConstructParam *properties)
{
	GObject *object;
	GtkDataboxPlotPrivate *priv;
	guint i;

	/* Always chain up to the parent constructor */
	object = G_OBJECT_CLASS (gtk_databox_plot_parent_class)->constructor (gtype, n_properties, properties);
	priv = GTK_DATABOX_PLOT_PRIVATE(object);

	/* Resize by 1/2 points by default */
	if (priv->nb_points_resize_step == 0 || priv->nb_points_resize_step >= priv->nb_points)
		priv->nb_points_resize_step = (int)(0.5 * priv->nb_points);

	priv->X = g_new0 (gfloat, priv->nb_points);
	priv->Y = g_new0 (gpointer, priv->nb_plots);
	for (i=0; i<priv->nb_plots; i++) {
		gfloat *y = g_new0 (gfloat, priv->nb_points);
		GtkDataboxGraph *graph = gtk_databox_lines_new (
										priv->nb_points,
										priv->X,
										y,
										(GdkColor*)&(colors[i%NB_COLORS]),
										1);
		gtk_databox_graph_add (GTK_DATABOX (object), graph);
		priv->Y[i] = y;
	}

	if (priv->refresh_rate > 0)
		g_timeout_add(priv->refresh_rate, timeout_callback, (gpointer)object);

	return object;
}

static void
gtk_databox_plot_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	G_OBJECT_CLASS (gtk_databox_plot_parent_class)->finalize (object);
}

static void
gtk_databox_plot_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_DATABOX_IS_PLOT (object));
	GtkDataboxPlotPrivate *priv = GTK_DATABOX_PLOT_PRIVATE(object);

	switch (prop_id)
	{
	case PROP_NUM_PLOTS:
		priv->nb_plots = g_value_get_uint (value);
		break;
	case PROP_NUM_POINTS:
		priv->nb_points = g_value_get_uint (value);
		break;
	case PROP_NUM_POINTS_RESIZE_STEP:
		priv->nb_points_resize_step = g_value_get_uint (value);
		break;
	case PROP_REFRESH_RATE:
		priv->refresh_rate = g_value_get_uint (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gtk_databox_plot_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
	g_return_if_fail (GTK_DATABOX_IS_PLOT (object));

	switch (prop_id)
	{
	case PROP_NUM_PLOTS:
		break;
	case PROP_NUM_POINTS:
		break;
	case PROP_NUM_POINTS_RESIZE_STEP:
		break;
	case PROP_REFRESH_RATE:
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gtk_databox_plot_class_init (GtkDataboxPlotClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkDataboxClass* parent_class = GTK_DATABOX_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GtkDataboxPlotPrivate));

	object_class->finalize = gtk_databox_plot_finalize;
	object_class->constructor = gtk_databox_plot_constructor;
	object_class->set_property = gtk_databox_plot_set_property;
	object_class->get_property = gtk_databox_plot_get_property;

	g_object_class_install_property (object_class,
	                                 PROP_NUM_PLOTS,
	                                 g_param_spec_uint ("num-plots",
	                                                    "num plots",
	                                                    "number of subplots",
	                                                    1,
	                                                    20,
	                                                    1,
	                                                    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
	                                 PROP_NUM_POINTS,
	                                 g_param_spec_uint ("num-points",
	                                                    "num data points",
	                                                    "number of datapoints",
	                                                    100,
	                                                    G_MAXUINT,
	                                                    2000,
	                                                    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
	                                 PROP_NUM_POINTS_RESIZE_STEP,
	                                 g_param_spec_uint ("num-points-resize-step",
	                                                    "num data resize step",
	                                                    "number of datapoints to resize by when full",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    0,
	                                                    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property (object_class,
	                                 PROP_REFRESH_RATE,
	                                 g_param_spec_uint ("refresh-rate",
	                                                    "refresh rate",
	                                                    "refresh rate ms",
	                                                    0,
	                                                    G_MAXUINT,
	                                                    166,
	                                                    G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
}

void 
gtk_databox_plot_update(GtkDataboxPlot *plot, float* values) 
{
	g_return_if_fail (GTK_DATABOX_IS_PLOT (plot));
	GtkDataboxPlotPrivate *priv = GTK_DATABOX_PLOT_PRIVATE(plot);
	guint i, j;

	gfloat *X = priv->X;
	gpointer *Y = priv->Y;

	if (priv->nb_data >= priv->nb_points) {
		priv->nb_data -= priv->nb_points_resize_step;

		guint mem_to_move = (priv->nb_points - priv->nb_points_resize_step)*sizeof(gfloat);
		memmove(X, &(X[priv->nb_points_resize_step]), mem_to_move);
		g_debug("Moving %d bytes\n", mem_to_move);

		for (i=0; i<priv->nb_plots; i++) {
		  	gfloat* y = Y[i];
		  	memmove(y, &(y[priv->nb_points_resize_step]), mem_to_move);
		}
	}

	for (j=priv->nb_data; j< priv->nb_points; j++) {
		X[j] = priv->time;
		for (i=0; i<priv->nb_plots; i++) {
			gfloat* y = Y[i];
			y[j] = values[i];
		}
	}
	priv->time++;
	priv->nb_data++;
}

GtkWidget *
gtk_databox_plot_new (void)
{
	return g_object_new (GTK_DATABOX_TYPE_PLOT, NULL);
}


