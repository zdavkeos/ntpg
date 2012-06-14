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
    brief = new Gtk::Grid();

    // grid settings
    brief->set_column_homogeneous(true);
    brief->set_column_spacing(10);

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
    ver_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    ver_ver_lbl.set_alignment(Gtk::ALIGN_START);
    pid_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    pid_pid_lbl.set_alignment(Gtk::ALIGN_START);
    path_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    path_path_lbl.set_alignment(Gtk::ALIGN_START);
    cmdline_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    cmdline_cmdline_lbl.set_alignment(Gtk::ALIGN_START);
    uptime_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    uptime_uptime_lbl.set_alignment(Gtk::ALIGN_START);
    start_lbl.set_alignment(Gtk::ALIGN_END, Gtk::ALIGN_CENTER);
    start_start_lbl.set_alignment(Gtk::ALIGN_START);

    
    // build brief summary pane
    brief->add(ver_lbl);
    brief->attach_next_to(ver_ver_lbl, ver_lbl, Gtk::POS_RIGHT, 1, 1);

    brief->attach_next_to(pid_lbl, ver_lbl, Gtk::POS_BOTTOM, 1, 1);
    brief->attach_next_to(pid_pid_lbl, pid_lbl, Gtk::POS_RIGHT, 1, 1);

    brief->attach_next_to(path_lbl, pid_lbl, Gtk::POS_BOTTOM, 1, 1);
    brief->attach_next_to(path_path_lbl, path_lbl, Gtk::POS_RIGHT, 1, 1);

    brief->attach_next_to(cmdline_lbl, path_lbl, Gtk::POS_BOTTOM, 1, 1);
    brief->attach_next_to(cmdline_cmdline_lbl, cmdline_lbl, Gtk::POS_RIGHT, 1, 1);

    brief->attach_next_to(uptime_lbl, cmdline_lbl, Gtk::POS_BOTTOM, 1, 1);
    brief->attach_next_to(uptime_uptime_lbl, uptime_lbl, Gtk::POS_RIGHT, 1, 1);

    brief->attach_next_to(start_lbl, uptime_lbl, Gtk::POS_BOTTOM, 1, 1);
    brief->attach_next_to(start_start_lbl, start_lbl, Gtk::POS_RIGHT, 1, 1);


    // pack everything else together
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
