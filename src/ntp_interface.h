/**
 * @file   ntpg_interface.h
 * @author Zach Davis
 * @date   Sep 01, 2011
 * 
 * @brief  Methods to interface ntpd
 * 
 * 
 */

#ifndef NTPG_INTERFACE_H
#define NTPG_INTERFACE_H

#include <gtkmm.h>

class ntpd_interface
{
  public:
    static gint read_ntp_conf(Glib::ustring &, Glib::ustring &);
    static gint get_peers_list(std::vector<std::string>&);
    static gboolean ntpd_get_version(Glib::ustring &);
    static gint ntpd_get_pid();
    static gint ntpd_get_location(Glib::ustring &);
    static gint ntpd_get_cmdline(guint ppid, Glib::ustring &cmdline);
    static gint ntpd_get_nice(guint ppid, Glib::ustring &nice);
    static gint ntpd_get_start_time(guint ppid, Glib::ustring &);
    static gint ntpd_get_up_time(guint ppid, Glib::ustring &);

};

#endif
