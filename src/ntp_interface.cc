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

#if 0
#include <glibtop.h>
#include <glibtop/procargs.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>
#endif

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
    std::string cmd = "ntpq -p";
    int retval;

    pl.clear();

    if(!app_config.use_dns) {
        cmd += " -n";
    }

    if(!app_config.use_local) {
        cmd += " " + app_config.remote_host;
    }

    // Glib::spawn_async_with_pipes
    // Glib::signal_child_watch().connect(sigc::ptr_fun(&child_watch_handler), pid);
    Glib::spawn_command_line_sync(cmd, &stdout, &stderr, &retval);

    // quick sanity check
    if(std::string::npos != stderr.find("timed out", 0)) {
        return -1;
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
    size_t retval;

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

    // we didn't get a version, but its not a show stopper
    ver = "Unknown";
    return TRUE;
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
    size_t retval;

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

/* libgtop based functions */

gint ntpd_interface::ntpd_get_cmdline(guint ppid, Glib::ustring &cmdline)
{
#if 0
    glibtop_proc_args args;

    cmdline = glibtop_get_proc_args(&args, ppid, 1024);

    return true;

#endif
    return false;
}

gint ntpd_interface::ntpd_get_nice(guint ppid, Glib::ustring &nice)
{
#if 0
    glibtop_proc_uid puid;
    std::stringstream ss;

    glibtop_get_proc_uid(&puid, ppid);
    ss << puid.nice;

    nice = ss.str();

    return true;
#endif
    return false;
}

gint ntpd_interface::ntpd_get_start_time(guint ppid, Glib::ustring &start)
{
#if 0
    glibtop_proc_time ptime;

    glibtop_get_proc_time(&ptime, ppid);
    start = ctime((time_t*)&ptime.start_time);
    
    // ctime adds a newline that gets in the way
    start.erase(start.length() - 1);

    return true;
#endif
    return false;
}

gint ntpd_interface::ntpd_get_up_time(guint ppid, Glib::ustring &start)
{
#if 0
    glibtop_proc_time ptime;
    std::stringstream ss;

    glibtop_get_proc_time(&ptime, ppid);
    ss << ptime.rtime;

    start = ss.str();

    return true;

#endif
    return false;
}
