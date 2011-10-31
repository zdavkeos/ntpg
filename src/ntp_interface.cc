/**
 * @file   ntpg_interface.cc
 * @author Zach Davis
 * @date   Sep 01, 2011
 * 
 * @brief  interface to ntpd
 * 
 * 
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <time.h>

#include <glibtop.h>
#include <glibtop/procargs.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>

#include "ntp_interface.h"
#include "util.h"


gint ntpd_interface::read_ntp_conf(Glib::ustring &file, Glib::ustring &str)
{
    if(file == "") {
        std::cout << "Error: passing empty string to read_ntp_conf" << std::endl;
        return -1;
    }

    str = Glib::file_get_contents(file);
    return 0;
}

gint ntpd_interface::get_peers_list(std::vector<std::string> &pl)
{
    std::string stdout;
    std::string stderr;
    int retval;

    pl.clear();

    if(app_config.use_dns) {
        Glib::spawn_command_line_sync("ntpq -p", &stdout, &stderr, &retval);
        // Glib::spawn_async_with_pipes
        // Glib::signal_child_watch().connect(sigc::ptr_fun(&child_watch_handler), pid);
    } else {
        Glib::spawn_command_line_sync("ntpq -pn", &stdout, &stderr, &retval);
    }
    
    // split the string into lines
    std::stringstream ss(stdout);
    ss << '\n'; // ntpq doesn't add a last newline...
    
    while(ss.good()) {
        std::string s;
        getline(ss, s, '\n');
        pl.push_back(s);
    }

    // throw away the first two lines
    pl.erase(pl.begin());
    pl.erase(pl.begin());
    pl.erase(pl.begin());
    pl.erase(pl.end());

    return 0;
}

gboolean ntpd_interface::ntpd_get_version(Glib::ustring &ver)
{
    std::string stdout;
    std::string stderr;
    int spawn_ret;
    guint retval;

    try {
        Glib::spawn_command_line_sync("ntpd -!", &stdout, &stderr, &spawn_ret);
    } 
    catch(Glib::SpawnError e) { return FALSE; }
    catch(Glib::ShellError e) { return FALSE; }
    catch(...) { return FALSE; }
   
    retval = stdout.find("Ver. ", 0, 5);
    if(retval != std::string::npos)
    {
        std::string tmp = stdout.substr(retval + 5, std::string::npos);
        tmp = trim_wsp(tmp);
        ver.clear();
        ver = tmp;
        return TRUE;
    }

    return FALSE;
}

gint ntpd_interface::ntpd_get_pid()
{
    std::string stdout;
    std::string stderr;
    int retval;
    gint pid;

    try {
        Glib::spawn_command_line_sync("pidof ntpd", &stdout, &stderr, &retval);
        std::stringstream ss(stdout);
        ss >> pid;
    } 
    //catch(Glib::SpawnError e) { return -1; }
    //catch(Glib::ShellError e) { return -1; }
    catch(...) {
        std::cout << "Unable to run pidof.";
        return -1; 
    }
   
    return pid;
}

gint ntpd_interface::ntpd_get_location(Glib::ustring &path)
{
    std::string stdout;
    std::string stderr;
    int spawn_ret;
    guint retval;

    try {
        Glib::spawn_command_line_sync("whereis -b ntpd", &stdout, &stderr, &spawn_ret);
    } 
    // catch(Glib::SpawnError e) { return false; }
    // catch(Glib::ShellError e) { return false; }
    catch(...) {
        std::cout << "Unable to run whereis.";
        return -1; 
    }

    retval = stdout.find("ntpd: ", 0, 6);
    if(retval != std::string::npos)
    {
        std::string tmp = stdout.substr(retval + 6, std::string::npos);
        tmp = trim_wsp(tmp);
        path.clear();
        path = tmp;
        return true;
    }
   
    return false;
}

gint ntpd_interface::ntpd_get_cmdline(guint ppid, Glib::ustring &cmdline)
{
#if 0
    /* non-libgtop style */
    Glib::ustring path("/proc/");
    std::stringstream ss;

    ss << ppid;
    path.append(ss.str());
    path.append("/cmdline");

    try {
        cmdline = Glib::file_get_contents(path);
    } catch(...) {
        std::cout << "ERROR: unable to open: " << path << std::endl;
        return 0;
    }
#endif

    glibtop_proc_args args;

    cmdline = glibtop_get_proc_args(&args, ppid, 1024);

    return true;
}

gint ntpd_interface::ntpd_get_nice(guint ppid, Glib::ustring &nice)
{
    glibtop_proc_uid puid;
    std::stringstream ss;

    glibtop_get_proc_uid(&puid, ppid);
    ss << puid.nice;

    nice = ss.str();

    return true;
}

gint ntpd_interface::ntpd_get_start_time(guint ppid, Glib::ustring &start)
{
    glibtop_proc_time ptime;

    glibtop_get_proc_time(&ptime, ppid);
    start = ctime((time_t*)&ptime.start_time);

    return true;
}

gint ntpd_interface::ntpd_get_up_time(guint ppid, Glib::ustring &start)
{
    glibtop_proc_time ptime;
    std::stringstream ss;

    glibtop_get_proc_time(&ptime, ppid);
    ss << ptime.rtime;

    start = ss.str();

    return true;
}
