/**
 * @file   ntpg_util.cc
 * @author Zach Davis
 * @date   Sep 13, 2011
 * 
 * @brief  Utility functions
 * 
 * 
 */

#include <iostream>

#include "util.h"

KeyVal::KeyVal()
    : Gtk::Table(1, 2, true)
{
    key.set_alignment(1.0, 0.5);
    val.set_alignment(1.0, 0.5);

    attach(key, 0, 1, 0, 1);
    attach(val, 1, 2, 0, 1);
}

KeyVal::KeyVal(Glib::ustring k, Glib::ustring v)
{
    attach(key, 0, 1, 0, 1);
    attach(val, 1, 2, 0, 1);

    key.set_label(k);
    val.set_label(v);
}

void KeyVal::set(Glib::ustring k, Glib::ustring v)
{
    key.set_label(k);
    val.set_label(v);
}

KeyValTable::KeyValTable()
    : Gtk::Table(1, 2, true)
{
    
}

// global config structure
ntpg_config app_config;

void set_default_config()
{
    app_config.file_font = "monospace 10";
    app_config.use_dns   = false;
    app_config.editor    = "gedit";
    app_config.default_conf_file = "/etc/ntp.conf";
}

std::string trim_wsp(std::string &s)
{
    const char *wsp = " \t\n\r";

    s.erase(0, s.find_first_not_of(wsp));
    s.erase(s.find_last_not_of(wsp) + 1);
    return s;
}

std::vector<Glib::ustring> *get_file_lines(Glib::ustring &file)
{
    std::vector<Glib::ustring> *lst;
    Glib::ustring contents;

    try {
        contents = Glib::file_get_contents(file);
    } catch(...) {
        std::cout << "Unable to open stats file: " << file << std::endl;
        return NULL;
    }

    lst = new std::vector<Glib::ustring>();
    std::stringstream ss(contents);

    while(ss.good()) {
        std::string s;
        getline(ss, s, '\n');
        lst->push_back(s);
    }

    return lst;
}

bool is_ntp_conf_keyword(Glib::ustring str)
{
#define NUM_KEYWORDS 10
    const char *keywords[] = {
    "driftfile", "restrict", "server", "fudge",
    "includefile", "crypto", "keys", "trustedkey",
    "controlkey", "statistics"};

    int i;

    for (i = 0; i < NUM_KEYWORDS; i++) {
        if (keywords[i] == str)
            return true;
    }

    return false;
}
