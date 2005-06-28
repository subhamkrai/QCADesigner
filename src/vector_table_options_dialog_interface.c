//////////////////////////////////////////////////////////
// QCADesigner                                          //
// Copyright 2002 Konrad Walus                          //
// All Rights Reserved                                  //
// Author: Konrad Walus                                 //
// Email: qcadesigner@gmail.com                         //
// WEB: http://qcadesigner.ca/                          //
//////////////////////////////////////////////////////////
//******************************************************//
//*********** PLEASE DO NOT REFORMAT THIS CODE *********//
//******************************************************//
// If your editor wraps long lines disable it or don't  //
// save the core files that way. Any independent files  //
// you generate format as you wish.                     //
//////////////////////////////////////////////////////////
// Please use complete names in variables and fucntions //
// This will reduce ramp up time for new people trying  //
// to contribute to the project.                        //
//////////////////////////////////////////////////////////
// This file was contributed by Gabriel Schulhof        //
// (schulhof@atips.ca).                                 //
//////////////////////////////////////////////////////////
// Contents:                                            //
//                                                      //
//                                                      //
//////////////////////////////////////////////////////////

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "support.h"
#include "custom_widgets.h"
#include "global_consts.h"
#include "bus_layout_dialog.h"
#include "vector_table_options_dialog_data.h"
#include "vector_table_options_dialog_callbacks.h"
#include "vector_table_options_dialog_interface.h"

void create_vector_table_options_dialog (vector_table_options_D *dialog)
  {
  GtkWidget *tbl = NULL, *toolbar = NULL, *btn = NULL, *btnBaseRadioSource = NULL ;
  GtkAccelGroup *accel_group = NULL ;
  GtkTreeViewColumn *col = NULL ;
  GtkCellRenderer *cr = NULL ;

  accel_group = gtk_accel_group_new () ;

  dialog->dialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_modal (GTK_WINDOW (dialog->dialog), TRUE);
  gtk_window_set_title (GTK_WINDOW (dialog->dialog), _("Vector Table Setup"));
  gtk_window_set_resizable (GTK_WINDOW (dialog->dialog), TRUE) ;
  g_object_set_data (G_OBJECT (dialog->dialog), "dialog", dialog) ;

  tbl = gtk_table_new (2, 1, FALSE) ;
  gtk_widget_show (tbl) ;
  gtk_container_add (GTK_CONTAINER (dialog->dialog), tbl) ;

  toolbar = gtk_toolbar_new () ;
  gtk_widget_show (toolbar) ;
  gtk_table_attach (GTK_TABLE (tbl), toolbar, 0, 1, 0, 1,
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
    (GtkAttachOptions)(GTK_FILL), 0, 0) ;
  gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_HORIZONTAL) ;
  gtk_toolbar_set_tooltips (GTK_TOOLBAR (toolbar), TRUE) ;
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH) ;

  g_object_set_data (G_OBJECT (btn = gtk_toolbar_append_element (
    GTK_TOOLBAR (toolbar),
    GTK_TOOLBAR_CHILD_BUTTON,
    NULL,
    _("Close"),
    _("Close Window"),
    _("Close vector table editor."),
    gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_LARGE_TOOLBAR),
    (GCallback)vector_table_options_dialog_btnClose_clicked,
    NULL)),
  "dialog", dialog) ;
	gtk_widget_add_accelerator (btn, "clicked", accel_group, GDK_w, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE) ;

  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)) ;

#ifdef STDIO_FILEIO
  dialog->btnOpen =
  btn = gtk_toolbar_append_element (
    GTK_TOOLBAR (toolbar),
    GTK_TOOLBAR_CHILD_BUTTON,
    NULL,
    _("Open"),
    _("Open Simulation Results"),
    _("Open and display another set of simulation results."),
    gtk_image_new_from_stock (GTK_STOCK_OPEN, GTK_ICON_SIZE_LARGE_TOOLBAR),
    (GCallback)vector_table_options_dialog_btnOpen_clicked,
    dialog) ;
	gtk_widget_add_accelerator (btn, "clicked", accel_group, GDK_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE) ;

  dialog->btnSave =
  btn = gtk_toolbar_append_element (
    GTK_TOOLBAR (toolbar),
    GTK_TOOLBAR_CHILD_BUTTON,
    NULL,
    _("Save"),
    _("Save Simulation Results"),
    _("Save the displayed simulation results."),
    gtk_image_new_from_stock (GTK_STOCK_SAVE, GTK_ICON_SIZE_LARGE_TOOLBAR),
    (GCallback)vector_table_options_dialog_btnSave_clicked,
    dialog->dialog) ;
	gtk_widget_add_accelerator (btn, "clicked", accel_group, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE) ;
#endif /* def STDIO_FILEIO */

  gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)) ;

  g_object_set_data (G_OBJECT (
    dialog->tbtnExhaustive =
    btnBaseRadioSource = gtk_toolbar_append_element (
      GTK_TOOLBAR (toolbar),
      GTK_TOOLBAR_CHILD_RADIOBUTTON,
      NULL,
      _("Exhaustive"),
      _("Exhaustive Verification"),
      _("Attempt all possible inputs."),
      gtk_image_new_from_stock (GTK_STOCK_YES, GTK_ICON_SIZE_LARGE_TOOLBAR),
      (GCallback)vector_table_options_dialog_btnSimType_clicked,
      dialog)),
    "sim_type", (gpointer)EXHAUSTIVE_VERIFICATION) ;

  g_object_set_data (G_OBJECT (
    dialog->tbtnVT =
    gtk_toolbar_append_element (
      GTK_TOOLBAR (toolbar),
      GTK_TOOLBAR_CHILD_RADIOBUTTON,
      btnBaseRadioSource,
      _("Vector Table"),
      _("Vector Table Simulation"),
      _("Create a sequence of inputs."),
      gtk_image_new_from_stock (GTK_STOCK_NO, GTK_ICON_SIZE_LARGE_TOOLBAR),
      (GCallback)vector_table_options_dialog_btnSimType_clicked,
      dialog)),
    "sim_type", (gpointer)VECTOR_TABLE) ;

  dialog->tblVT = gtk_table_new (1, 2, FALSE) ;
  gtk_widget_show (dialog->tblVT) ;
  gtk_table_attach (GTK_TABLE (tbl), dialog->tblVT, 0, 1, 1, 2,
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 0, 0) ;

  toolbar = gtk_toolbar_new () ;
  gtk_widget_show (toolbar) ;
  gtk_table_attach (GTK_TABLE (dialog->tblVT), toolbar, 0, 1, 0, 1,
    (GtkAttachOptions)(GTK_FILL),
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 0, 0) ;
  gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_VERTICAL) ;
  gtk_toolbar_set_tooltips (GTK_TOOLBAR (toolbar), TRUE) ;
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS) ;

  btn = gtk_toolbar_append_element (
    GTK_TOOLBAR (toolbar),
    GTK_TOOLBAR_CHILD_BUTTON,
    NULL,
    _("Add"),
    _("Add Vector"),
    _("Apend a vector to the end of the table."),
    gtk_image_new_from_stock (GTK_STOCK_ADD, GTK_ICON_SIZE_LARGE_TOOLBAR),
    (GCallback)vector_table_options_dialog_btnAdd_clicked,
    dialog->dialog) ;

  dialog->sw = gtk_scrolled_window_new (
    GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 1, 1, 1, 1)), 
    GTK_ADJUSTMENT (gtk_adjustment_new (0, 0, 1, 1, 1, 1))) ;
  gtk_widget_show (dialog->sw) ;
  gtk_table_attach (GTK_TABLE (dialog->tblVT), dialog->sw, 1, 2, 0, 1,
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
    (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 2, 2) ;

  dialog->tv = create_bus_layout_tree_view (TRUE, _("Inputs"), GTK_SELECTION_SINGLE) ;
  gtk_tree_view_append_column (GTK_TREE_VIEW (dialog->tv), col = gtk_tree_view_column_new ()) ;
  gtk_tree_view_column_set_title (col, _("Active")) ;
  gtk_tree_view_column_pack_start (col, cr = dialog->crActive = gtk_cell_renderer_toggle_new (), TRUE) ;
  gtk_tree_view_column_add_attribute (col, cr, "active", VECTOR_TABLE_MODEL_COLUMN_ACTIVE) ;
  g_object_set (G_OBJECT (cr), "activatable", TRUE, NULL) ;
  gtk_cell_renderer_toggle_set_active (GTK_CELL_RENDERER_TOGGLE (cr), TRUE) ;
  gtk_widget_show (dialog->tv) ;
  gtk_container_add (GTK_CONTAINER (dialog->sw), dialog->tv) ;

  g_signal_connect (G_OBJECT (dialog->tv),     "style-set",    (GCallback)tree_view_style_set,                          NULL) ;
  g_signal_connect (G_OBJECT (cr),             "toggled",      (GCallback)vt_model_active_toggled,                      dialog->tv) ;
  g_signal_connect (G_OBJECT (dialog->dialog), "delete-event", (GCallback)vector_table_options_dialog_btnClose_clicked, NULL) ;

  gtk_window_add_accel_group (GTK_WINDOW (dialog->dialog), accel_group) ;
  }

void add_vector_to_dialog (vector_table_options_D *dialog, VectorTable *pvt, int idx)
  {
  GtkTreeViewColumn *col = NULL ;
  GtkCellRenderer *cr = NULL ;
  int idxCol = (-1 == idx ? idx : idx + 2) ;
  int idxVector = (-1 == idx ? pvt->vectors->icUsed - 1 : idx) ;
  char *psz = NULL ;

  gtk_tree_view_insert_column (GTK_TREE_VIEW (dialog->tv), col = gtk_tree_view_column_new (), idxCol) ;
  gtk_tree_view_column_set_title (col, psz = g_strdup_printf ("%d", idxVector)) ;
  g_free (psz) ;
  gtk_tree_view_column_pack_start (col, cr = gtk_cell_renderer_text_new (), FALSE) ;
  g_object_set (G_OBJECT (cr), 
    "cell-background-gdk", &((gtk_widget_get_style (dialog->tv))->base[3]),
    "cell-background-set", FALSE, 
    "editable", TRUE, NULL) ;
  gtk_tree_view_column_set_clickable (col, TRUE) ;
  g_object_set_data (G_OBJECT (cr), "idxVector", (gpointer)idxVector) ;
  g_object_set_data (G_OBJECT (cr), "pvt", pvt) ;
  gtk_tree_view_column_set_cell_data_func (col, cr, vector_data_func, pvt, NULL) ;
  g_object_set_data (G_OBJECT (col), "cr", cr) ;

  g_signal_connect (G_OBJECT (col), "clicked", (GCallback)vector_column_clicked, dialog) ;
  g_signal_connect (G_OBJECT (cr), "edited",  (GCallback)vector_value_edited,   dialog->tv) ;

  gtk_tree_view_column_clicked (col) ;
  }
