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
// Contents:                                            //
//                                                      //
// This UI allows the user to specify parameters for    //
// the coherence vector simulation engine.              //
//                                                      //
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "support.h"
#include "global_consts.h"
#include "coherence_vector_properties_dialog.h"

#define DBG_RO(s)

typedef struct
  {
  GtkWidget *coherence_properties_dialog;
  GtkWidget *dialog_vbox1;
  GtkWidget *table;
  GtkWidget *T_entry;
  GtkWidget *relaxation_entry;
  GtkWidget *time_step_entry;
  GtkWidget *duration_entry;
  GtkWidget *clock_high_entry;
  GtkWidget *clock_low_entry;
  GtkWidget *clock_shift_entry;
  //Added by Marco March
  GtkWidget *lbljitph0;
  GtkWidget *lbljitph1;
  GtkWidget *lbljitph2;
  GtkWidget *lbljitph3;
  //End Added by Marco
  GtkWidget *clock_amplitude_factor_entry;
  GtkWidget *radius_of_effect_entry;
  GtkWidget *epsilonR_entry;
	GtkWidget *layer_separation_entry;
	  /*
	GtkWidget *euler_method_radio;
	GtkWidget *runge_kutta_radio;
	   */
    GtkWidget *chkStepSize;  
	GtkWidget *chkCorrelations;
	GtkWidget *chkAnimate;
  GtkWidget *dialog_action_area1;
  GtkWidget *hbox2;
  GtkWidget *coherence_properties_ok_button;
  GtkWidget *coherence_properties_cancel_button;
  GSList    *radio_group;
//Added by Marco March 
  GtkWidget *jitter_phase_0_entry;
  GtkWidget *jitter_phase_1_entry;
  GtkWidget *jitter_phase_2_entry;
  GtkWidget *jitter_phase_3_entry;
//End addded by Marco
//Added by Faizal
  GtkWidget *wave_number_kx_entry;
  GtkWidget *wave_number_ky_entry;
	  GtkWidget *num_iterations;
  GtkWidget *cont_clocking_radio;
  GtkWidget *four_phase_clocking_radio;
  GSList    *clock_radio_group;
	  GtkWidget *error_thresh_entry;
//End added by Faizal
  } coherence_properties_D;

static coherence_properties_D coherence_properties = {NULL};

static void create_coherence_properties_dialog (coherence_properties_D *dialog) ;
static void coherence_OP_to_dialog (coherence_OP *psco, coherence_properties_D *dialog) ;
static void dialog_to_coherence_OP (coherence_OP *psco, coherence_properties_D *dialog) ;
static void create_coherence_properties_line (GtkWidget *table, int idx, GtkWidget **plabel, GtkWidget **pentry, GtkWidget **plblUnits, char *pszLabel, char *pszUnits, gboolean bEnableEntry) ;
static void properties_changed (GtkWidget *widget, gpointer user_data) ;

void get_coherence_properties_from_user (GtkWindow *parent, coherence_OP *pbo)
  {
  coherence_OP scoLocal = {0} ;

  if (NULL == coherence_properties.coherence_properties_dialog)
    create_coherence_properties_dialog (&coherence_properties) ;

  gtk_window_set_transient_for (GTK_WINDOW (coherence_properties.coherence_properties_dialog), parent) ;

  g_object_set_data (G_OBJECT (coherence_properties.coherence_properties_dialog), "bIgnoreChangeSignal", (gpointer)TRUE) ;
  coherence_OP_to_dialog (pbo, &coherence_properties) ;
  dialog_to_coherence_OP (&scoLocal, &coherence_properties) ;
  coherence_OP_to_dialog (&scoLocal, &coherence_properties) ;
  g_object_set_data (G_OBJECT (coherence_properties.coherence_properties_dialog), "bIgnoreChangeSignal", (gpointer)FALSE) ;

  if (GTK_RESPONSE_OK == gtk_dialog_run (GTK_DIALOG (coherence_properties.coherence_properties_dialog)))
    dialog_to_coherence_OP (pbo, &coherence_properties) ;

  gtk_widget_hide (coherence_properties.coherence_properties_dialog) ;
  }

static void create_coherence_properties_dialog (coherence_properties_D *dialog)
  {
  GtkWidget *label = NULL, *lblunits = NULL, *frm = NULL, *tblAlgo = NULL ;

  if (NULL != dialog->coherence_properties_dialog) return ;

  dialog->coherence_properties_dialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (dialog->coherence_properties_dialog), _("Coherence Vector Options"));
  gtk_window_set_resizable (GTK_WINDOW (dialog->coherence_properties_dialog), FALSE);
  gtk_window_set_modal (GTK_WINDOW (dialog->coherence_properties_dialog), TRUE) ;

  dialog->dialog_vbox1 = GTK_DIALOG (dialog->coherence_properties_dialog)->vbox;
  gtk_widget_show (dialog->dialog_vbox1);

  dialog->table = gtk_table_new (19, 3, FALSE);
  gtk_widget_show (dialog->table);
  gtk_container_set_border_width (GTK_CONTAINER (dialog->table), 2);
  gtk_box_pack_start (GTK_BOX (dialog->dialog_vbox1), dialog->table, TRUE, TRUE, 0);

  create_coherence_properties_line (dialog->table,  0, &(label), &(dialog->T_entry),                      &lblunits, _("Temperature:"),            "K",  TRUE) ;
  create_coherence_properties_line (dialog->table,  1, &(label), &(dialog->relaxation_entry),             &lblunits, _("Relaxation Time:"),        "s",  TRUE) ;
  create_coherence_properties_line (dialog->table,  2, &(label), &(dialog->time_step_entry),              &lblunits, _("Time Step:"),              "s",  TRUE) ;
  create_coherence_properties_line (dialog->table,  3, &(label), &(dialog->duration_entry),               &lblunits, _("Total Simulation Time:"),  "s",  TRUE) ;
  create_coherence_properties_line (dialog->table,  4, &(label), &(dialog->clock_high_entry),             &lblunits, _("Clock High:"),             "J",  TRUE) ;
  create_coherence_properties_line (dialog->table,  5, &(label), &(dialog->clock_low_entry),              &lblunits, _("Clock Low:"),              "J",  TRUE) ;
  create_coherence_properties_line (dialog->table,  7, &(label), &(dialog->clock_shift_entry),            &lblunits, _("Clock Shift:"),            NULL, TRUE) ;
  create_coherence_properties_line (dialog->table,  8, &(label), &(dialog->clock_amplitude_factor_entry), &lblunits, _("Clock Amplitude Factor:"), NULL, TRUE) ;
  create_coherence_properties_line (dialog->table,  9, &(label), &(dialog->radius_of_effect_entry),       &lblunits, _("Radius of Effect:"),       "nm", TRUE) ;
  create_coherence_properties_line (dialog->table, 10, &(label), &(dialog->epsilonR_entry),               NULL,      _("Relative Permittivity:"),  NULL, TRUE) ;
  create_coherence_properties_line (dialog->table, 11, &(label), &(dialog->layer_separation_entry),       &lblunits, _("Layer Separation:"),       "nm", TRUE) ;
  create_coherence_properties_line (dialog->table, 12, &(label), &(dialog->error_thresh_entry),           &lblunits, _("Tolerance:"),              NULL, TRUE) ;
//Added by Marco
  create_coherence_properties_line (dialog->table, 13, &(label), &(dialog->jitter_phase_0_entry),         &lblunits, _("Phase Shift Clock 0:"), _("degrees"), TRUE) ;
  create_coherence_properties_line (dialog->table, 14, &(label), &(dialog->jitter_phase_1_entry),         &lblunits, _("Phase Shift Clock 1:"), _("degrees"), TRUE) ;
  create_coherence_properties_line (dialog->table, 15, &(label), &(dialog->jitter_phase_2_entry),         &lblunits, _("Phase Shift Clock 2:"), _("degrees"), TRUE) ;
  create_coherence_properties_line (dialog->table, 16, &(label), &(dialog->jitter_phase_3_entry),         &lblunits, _("Phase Shift Clock 3:"), _("degrees"), TRUE) ;
//End added by Marco
//Added by Faizal
  create_coherence_properties_line (dialog->table, 17, &(label), &(dialog->wave_number_kx_entry),         &lblunits, _("Wave Number, kx:"), "1/nm", TRUE) ;
  create_coherence_properties_line (dialog->table, 18, &(label), &(dialog->wave_number_ky_entry),         &lblunits, _("Wave Number, ky:"), "1/nm", TRUE) ;
  create_coherence_properties_line (dialog->table, 19, &(label), &(dialog->num_iterations),				  &lblunits, _("Number of Iterations:"), NULL, TRUE) ;
//End added by Faizal

	  
	  /*
  frm = gtk_frame_new (_("Algorithm")) ;
  gtk_widget_show (frm) ;
  gtk_table_attach (GTK_TABLE (dialog->table), frm, 0, 3, 18, 19, GTK_EXPAND | GTK_FILL, GTK_FILL, 2, 2) ;
  gtk_container_set_border_width (GTK_CONTAINER (frm), 2) ;

  tblAlgo = gtk_table_new (2, 1, FALSE) ;
  gtk_widget_show (tblAlgo) ;
  gtk_container_add (GTK_CONTAINER (frm), tblAlgo) ;
  gtk_container_set_border_width (GTK_CONTAINER (tblAlgo), 2) ;

  dialog->euler_method_radio = gtk_radio_button_new_with_label (dialog->radio_group, "Euler Method");
  g_object_set_data (G_OBJECT (dialog->euler_method_radio), "which_options", (gpointer)EULER_METHOD) ;
  dialog->radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog->euler_method_radio));
  gtk_widget_show (dialog->euler_method_radio);
  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->euler_method_radio, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 2, 2);

  dialog->runge_kutta_radio = gtk_radio_button_new_with_label (dialog->radio_group, "Runge Kutta");
  g_object_set_data (G_OBJECT (dialog->runge_kutta_radio), "which_options", (gpointer)RUNGE_KUTTA) ;
  dialog->radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog->runge_kutta_radio));
  gtk_widget_show (dialog->runge_kutta_radio);
  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->runge_kutta_radio, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 2, 2);

	   */
//Added by Faizal for cont. clocking
                    
  frm = gtk_frame_new (_("Clocking")) ;
  gtk_widget_show (frm) ;
  gtk_table_attach (GTK_TABLE (dialog->table), frm, 0, 3, 20, 21, GTK_EXPAND | GTK_FILL, GTK_FILL, 2, 2) ;
  gtk_container_set_border_width (GTK_CONTAINER (frm), 2) ;

  tblAlgo = gtk_table_new (2, 3, FALSE) ;
  gtk_widget_show (tblAlgo) ;
  gtk_container_add (GTK_CONTAINER (frm), tblAlgo) ;
  gtk_container_set_border_width (GTK_CONTAINER (tblAlgo), 2) ;

  dialog->cont_clocking_radio = gtk_radio_button_new_with_label (dialog->clock_radio_group, "Continuous Clocking");
  g_object_set_data (G_OBJECT (dialog->cont_clocking_radio), "which_options", (gpointer)CONT_CLOCKING) ;
  dialog->clock_radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog->cont_clocking_radio));
  gtk_widget_show (dialog->cont_clocking_radio);
  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->cont_clocking_radio, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 2, 2);

  dialog->four_phase_clocking_radio = gtk_radio_button_new_with_label (dialog->clock_radio_group, "4 Phase Clocking");
  g_object_set_data (G_OBJECT (dialog->four_phase_clocking_radio), "which_options", (gpointer)FOUR_PHASE_CLOCKING) ;
  dialog->clock_radio_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (dialog->four_phase_clocking_radio));
  gtk_widget_show (dialog->four_phase_clocking_radio);
  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->four_phase_clocking_radio, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 2, 2);                  

//End added by Faizal

	  frm = gtk_frame_new (_("ODE Parameters")) ;
	  gtk_widget_show (frm) ;
	  gtk_table_attach (GTK_TABLE (dialog->table), frm, 0, 3, 21, 24, GTK_EXPAND | GTK_FILL, GTK_FILL, 2, 2) ;
	  gtk_container_set_border_width (GTK_CONTAINER (frm), 2) ;
	  
	  tblAlgo = gtk_table_new (2, 3, FALSE) ;
	  gtk_widget_show (tblAlgo) ;
	  gtk_container_add (GTK_CONTAINER (frm), tblAlgo) ;
	  gtk_container_set_border_width (GTK_CONTAINER (tblAlgo), 2) ;
	  
	  // Use Adaptive Step Size ?
	  dialog->chkStepSize = gtk_check_button_new_with_label (_("Use Adaptive Step Size")) ;
	  gtk_widget_show (dialog->chkStepSize) ;
	  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->chkStepSize, 0, 1, 0, 1,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (GTK_FILL), 2, 2);
	  // Include Correlations ?
	  dialog->chkCorrelations = gtk_check_button_new_with_label (_("Include Correlations")) ;
	  gtk_widget_show (dialog->chkCorrelations) ;
	  gtk_table_attach (GTK_TABLE (tblAlgo), dialog->chkCorrelations, 0, 1, 1, 2,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (GTK_FILL), 2, 2);  
	   
	  
/*
dialog->chkStepSize = gtk_check_button_new_with_label (_("Use Adaptive Step Size")) ;
gtk_widget_show (dialog->chkStepSize) ;
gtk_table_attach (GTK_TABLE (dialog->table), dialog->chkStepSize, 0, 2, 21, 22,
                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                  (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);

  
  dialog->chkCorrelations = gtk_check_button_new_with_label (_("Include Correlations")) ;
  gtk_widget_show (dialog->chkCorrelations) ;
  gtk_table_attach (GTK_TABLE (dialog->table), dialog->chkCorrelations, 0, 2, 23, 24,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
					(GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);

 */
  // Animate ?
  dialog->chkAnimate = gtk_check_button_new_with_label (_("Animate")) ;
  gtk_widget_show (dialog->chkAnimate) ;
  gtk_table_attach (GTK_TABLE (dialog->table), dialog->chkAnimate, 0, 2, 24, 25,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);

  g_signal_connect (G_OBJECT (dialog->T_entry),                      "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->relaxation_entry),             "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->time_step_entry),              "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->duration_entry),               "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->clock_high_entry),             "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->clock_low_entry),              "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->clock_shift_entry),            "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->clock_amplitude_factor_entry), "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->radius_of_effect_entry),       "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->epsilonR_entry),               "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->layer_separation_entry),       "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->clock_amplitude_factor_entry), "changed", (GCallback)properties_changed, dialog) ;
//Added by Marco
  g_signal_connect (G_OBJECT (dialog->jitter_phase_0_entry),         "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->jitter_phase_1_entry),         "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->jitter_phase_2_entry),         "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->jitter_phase_3_entry),         "changed", (GCallback)properties_changed, dialog) ;
//End added by Marco
//Added by Faizal
  g_signal_connect (G_OBJECT (dialog->wave_number_kx_entry),            "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->wave_number_ky_entry),            "changed", (GCallback)properties_changed, dialog) ;
  g_signal_connect (G_OBJECT (dialog->num_iterations),					"changed", (GCallback)properties_changed, dialog) ;	  
//End added by Faizal
  g_signal_connect (G_OBJECT (dialog->error_thresh_entry),                      "changed", (GCallback)properties_changed, dialog) ;
  gtk_dialog_add_button (GTK_DIALOG (dialog->coherence_properties_dialog), GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL) ;
  gtk_dialog_add_button (GTK_DIALOG (dialog->coherence_properties_dialog), GTK_STOCK_OK,     GTK_RESPONSE_OK) ;
  gtk_dialog_set_default_response (GTK_DIALOG (dialog->coherence_properties_dialog), GTK_RESPONSE_OK) ;
  }

static void properties_changed (GtkWidget *widget, gpointer user_data)
  {
  coherence_properties_D *dialog = (coherence_properties_D *)user_data ;
  coherence_OP sco ;

  if ((gboolean)g_object_get_data (G_OBJECT (dialog->coherence_properties_dialog), "bIgnoreChangeSignal"))
    return ;

  dialog_to_coherence_OP (&sco, dialog) ;
  }

static void create_coherence_properties_line (GtkWidget *table, int idx, GtkWidget **plabel, GtkWidget **pentry, GtkWidget **plblUnits, char *pszLabel, char *pszUnits, gboolean bEnableEntry)
  {
  // Electron lifetime
  (*plabel) = gtk_label_new (_(pszLabel));
  gtk_widget_show ((*plabel));
  gtk_table_attach (GTK_TABLE (table), (*plabel), 0, 1, idx, idx + 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);
  gtk_label_set_justify (GTK_LABEL ((*plabel)), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC ((*plabel)), 1, 0.5);

  (*pentry) = gtk_entry_new ();
  gtk_widget_show ((*pentry));
  gtk_table_attach (GTK_TABLE (table), (*pentry), 1, 2, idx, idx + 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);
  gtk_entry_set_activates_default (GTK_ENTRY ((*pentry)), bEnableEntry) ;
  gtk_widget_set_sensitive ((*pentry), bEnableEntry) ;

  if (NULL != pszUnits)
    {
    (*plblUnits) = gtk_label_new (_(pszUnits));
    gtk_widget_show ((*plblUnits));
    gtk_table_attach (GTK_TABLE (table), (*plblUnits), 2, 3, idx, idx + 1,
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                      (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 2, 2);
    gtk_label_set_justify (GTK_LABEL ((*plblUnits)), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment (GTK_MISC ((*plblUnits)), 0, 0.5);
    }
  }

static void coherence_OP_to_dialog (coherence_OP *psco, coherence_properties_D *dialog)
  {
  char sz[16] = "" ;

  g_snprintf (sz, 16, "%f", psco->T) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->T_entry), sz) ;

  g_snprintf (sz, 16, "%e", psco->relaxation) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->relaxation_entry), sz) ;

  g_snprintf (sz, 16, "%e", psco->time_step) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->time_step_entry), sz) ;

  g_snprintf (sz, 16, "%e", psco->duration) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->duration_entry), sz) ;

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog->chkStepSize), psco->adaptive_step) ;    
      
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog->chkCorrelations), psco->include_correlations) ;

  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (dialog->chkAnimate), psco->animate_simulation) ;

  g_snprintf (sz, 16, "%e", psco->clock_high) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->clock_high_entry), sz) ;

  g_snprintf (sz, 16, "%e", psco->clock_low) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->clock_low_entry), sz) ;

  g_snprintf (sz, 16, "%e", psco->clock_shift) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->clock_shift_entry), sz) ;

  g_snprintf (sz, 16, "%lf", psco->clock_amplitude_factor) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->clock_amplitude_factor_entry), sz) ;

  g_snprintf (sz, 16, "%lf", psco->radius_of_effect) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->radius_of_effect_entry), sz) ;

  g_snprintf (sz, 16, "%lf", psco->epsilonR) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->epsilonR_entry), sz) ;

  g_snprintf (sz, 16, "%lf", psco->layer_separation) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->layer_separation_entry), sz) ;

//Added by Marco
  g_snprintf (sz, 16, "%lf", psco->jitter_phase_0) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->jitter_phase_0_entry), sz) ;
  
  g_snprintf (sz, 16, "%lf", psco->jitter_phase_1) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->jitter_phase_1_entry), sz) ;
  
  g_snprintf (sz, 16, "%lf", psco->jitter_phase_2) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->jitter_phase_2_entry), sz) ;
  
  g_snprintf (sz, 16, "%lf", psco->jitter_phase_3) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->jitter_phase_3_entry), sz) ;
//End added by Marco
//Added by Faizal
  g_snprintf (sz, 16, "%lf", psco->wave_number_kx) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->wave_number_kx_entry), sz) ;
  
  g_snprintf (sz, 16, "%lf", psco->wave_number_ky) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->wave_number_ky_entry), sz) ;
	  
  g_snprintf (sz, 16, "%ld", psco->num_iterations) ;
  gtk_entry_set_text (GTK_ENTRY (dialog->num_iterations), sz) ;	  
//End added by Faizal

	  g_snprintf (sz, 16, "%f", psco->error_thresh) ;
	  gtk_entry_set_text (GTK_ENTRY (dialog->error_thresh_entry), sz) ;
	  
	  /*
  if (EULER_METHOD == psco->algorithm)
  	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(dialog->euler_method_radio), TRUE);
  else
  if (RUNGE_KUTTA == psco->algorithm)
  	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(dialog->runge_kutta_radio), TRUE);
*/
//Added by Faizal for cont. clocking

  if (CONT_CLOCKING == psco->clocking)
  	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(dialog->cont_clocking_radio), TRUE);
  else
  if (FOUR_PHASE_CLOCKING == psco->clocking)
  	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(dialog->four_phase_clocking_radio), TRUE);

//End added by Faizal 
 }

static void dialog_to_coherence_OP (coherence_OP *psco, coherence_properties_D *dialog)
  {
  psco->T                      = atof (gtk_entry_get_text (GTK_ENTRY (dialog->T_entry))) ;
  psco->relaxation             = atof (gtk_entry_get_text (GTK_ENTRY (dialog->relaxation_entry))) ;
  psco->time_step              = atof (gtk_entry_get_text (GTK_ENTRY (dialog->time_step_entry))) ;
  psco->duration               = atof (gtk_entry_get_text (GTK_ENTRY (dialog->duration_entry))) ;
  psco->clock_high             = atof (gtk_entry_get_text (GTK_ENTRY (dialog->clock_high_entry))) ;
  psco->clock_low              = atof (gtk_entry_get_text (GTK_ENTRY (dialog->clock_low_entry))) ;
  psco->clock_shift            = atof (gtk_entry_get_text (GTK_ENTRY (dialog->clock_shift_entry))) ;
  psco->clock_amplitude_factor = atof (gtk_entry_get_text (GTK_ENTRY (dialog->clock_amplitude_factor_entry))) ;
  psco->radius_of_effect       = atof (gtk_entry_get_text (GTK_ENTRY (dialog->radius_of_effect_entry))) ;
  psco->epsilonR               = atof (gtk_entry_get_text (GTK_ENTRY (dialog->epsilonR_entry))) ;
  psco->layer_separation       = atof (gtk_entry_get_text (GTK_ENTRY (dialog->layer_separation_entry))) ;
//Added by Marco
  psco->jitter_phase_0       = atoi (gtk_entry_get_text (GTK_ENTRY (dialog->jitter_phase_0_entry))) ;
  psco->jitter_phase_1       = atoi (gtk_entry_get_text (GTK_ENTRY (dialog->jitter_phase_1_entry))) ;
  psco->jitter_phase_2       = atoi (gtk_entry_get_text (GTK_ENTRY (dialog->jitter_phase_2_entry))) ;
  psco->jitter_phase_3       = atoi (gtk_entry_get_text (GTK_ENTRY (dialog->jitter_phase_3_entry))) ;
//End added by Marco
//Added by Faizal
  psco->wave_number_kx       = atof (gtk_entry_get_text (GTK_ENTRY (dialog->wave_number_kx_entry))) ;
  psco->wave_number_ky       = atof (gtk_entry_get_text (GTK_ENTRY (dialog->wave_number_ky_entry))) ;
  psco->num_iterations       = atoi (gtk_entry_get_text (GTK_ENTRY (dialog->num_iterations))) ;
  psco->error_thresh			 = atof (gtk_entry_get_text (GTK_ENTRY (dialog->error_thresh_entry))) ;
//End added by Faizal
	/*
  psco->algorithm          = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->euler_method_radio)) ? EULER_METHOD : RUNGE_KUTTA;
	 */
  psco->clocking           = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->four_phase_clocking_radio)) ? FOUR_PHASE_CLOCKING : CONT_CLOCKING; //Added by Faizal for cont. clocking
  psco->adaptive_step    = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->chkStepSize)) ;
  psco->include_correlations    = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->chkCorrelations)) ;
  psco->animate_simulation = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (dialog->chkAnimate)) ;
  }
