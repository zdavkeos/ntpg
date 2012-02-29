/**
 * @file   ntpg_summary.cc
 * @author Zach Davis
 * @date   Sep 01, 2011
 * 
 * @brief  overview of ntpd state
 * 
 * 
 */

#include "summary.h"

ntpg_summary::ntpg_summary()
{
    Name = "Summary";
    Title = "Summary of ntpd state";
    // create any pointers to widgets
    brief = new Gtk::Table(5, 2, TRUE);

    // set static labels
    top_lbl.set_text("NTP Daemon Summary");
    ver_lbl.set_text("ntpd version:");
    pid_lbl.set_text("ntpd pid:");
    path_lbl.set_text("ntpd location:");
    cmdline_lbl.set_text("ntpd command line:");
    uptime_lbl.set_text("ntpd up time:");
    start_lbl.set_text("ntpd start:");

    // get dynamic label data (if available)
    get_ntp_data();
    ver_ver_lbl.set_text(ver_string);
    pid_pid_lbl.set_text(pid_string);
    path_path_lbl.set_text(path_string);
    cmdline_cmdline_lbl.set_text(cmdline_string);
    start_start_lbl.set_text(start_string);
    uptime_uptime_lbl.set_text(uptime_string);

    // set label justification...
    ver_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    ver_ver_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    pid_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    pid_pid_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    path_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    path_path_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    cmdline_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    cmdline_cmdline_lbl.set_justify(Gtk::JUSTIFY_RIGHT);

    uptime_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    uptime_uptime_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    start_lbl.set_justify(Gtk::JUSTIFY_RIGHT);
    start_start_lbl.set_justify(Gtk::JUSTIFY_RIGHT);

    
    // build brief summary pane
    brief->attach(ver_lbl, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(ver_ver_lbl, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(pid_lbl, 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(pid_pid_lbl, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(path_lbl, 0, 1, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(path_path_lbl, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(cmdline_lbl, 0, 1, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(cmdline_cmdline_lbl, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(uptime_lbl, 0, 1, 4, 5, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(uptime_uptime_lbl, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(start_lbl, 0, 1, 5, 6, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    brief->attach(start_start_lbl, 1, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK, 3, 3);


    // pack everything together
    attach(top_lbl, 0, 2, 0, 1, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK, 10, 10);
    attach(sep, 0, 2, 1, 2, Gtk::EXPAND | Gtk::FILL, Gtk::SHRINK, 3, 3);
    attach(*brief, 0, 2, 2, 3, Gtk::SHRINK, Gtk::EXPAND | Gtk::FILL, 10, 10);
}

void ntpg_summary::do_reload_conf()
{
    
}

gint ntpg_summary::get_ntp_data()
{
    gint count = 0;
    
    if(ntpd_interface::ntpd_get_version(ver_string))
        count++;

    gint pid = ntpd_interface::ntpd_get_pid();
    if(pid) {
        std::stringstream ss;
        ss << pid;
        pid_string = ss.str();
        count++;
    }

    if(ntpd_interface::ntpd_get_location(path_string))
        count++;
    
    if(ntpd_interface::ntpd_get_cmdline(pid, cmdline_string))
        count++;

    if(ntpd_interface::ntpd_get_up_time(pid, uptime_string))
        count++;

    if(ntpd_interface::ntpd_get_start_time(pid, start_string))
        count++;

    return count;
}
