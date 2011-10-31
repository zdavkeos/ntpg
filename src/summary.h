/**
  * @file   ntpg_summary.h
  * @author Zach Davis
  * @date   Sep 01, 2011
  * 
  * @brief  overview of ntpd state
  * 
  * 
  */

#ifndef NTPG_SUMMARY_H
#define NTPG_SUMMARY_H

#include <gtkmm.h>

#include "util.h"
#include "ntp_interface.h"

class ntpg_summary : public ntpg_page
{
  public:
    ntpg_summary();

    gint get_ntp_data();
    void do_reload_conf();

  private:
    Gtk::Label top_lbl;
    Gtk::Table *brief;
    Gtk::Label ver_lbl;
    Gtk::Label ver_ver_lbl;
    Gtk::Label pid_lbl;
    Gtk::Label pid_pid_lbl;
    Gtk::Label path_lbl;
    Gtk::Label path_path_lbl;
    Gtk::Label cmdline_lbl;
    Gtk::Label cmdline_cmdline_lbl;
    Gtk::Label uptime_lbl;
    Gtk::Label uptime_uptime_lbl;
    Gtk::Label start_lbl;
    Gtk::Label start_start_lbl;
    Gtk::HSeparator sep;

    // ntpd info
    Glib::ustring ver_string;
    Glib::ustring pid_string;
    Glib::ustring path_string;
    Glib::ustring cmdline_string;
    Glib::ustring uptime_string;
    Glib::ustring start_string;

};

#endif
