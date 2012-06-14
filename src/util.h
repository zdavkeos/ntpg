/**
 * @file   ntpg_util.h
 * @author Zach Davis
 * @date   Sep 13, 2011
 * 
 * @brief  ntpg utility functions
 * 
 * 
 */

#ifndef NTPG_UTIL_H
#define NTPG_UTIL_H

#include <vector>
#include <sstream>
#include <algorithm>

#include <gtkmm.h>

// global configuration structure
typedef struct ntpg_config
{
    Glib::ustring file_font;
    bool          use_dns;
    Glib::ustring editor;
    Glib::ustring default_conf_file;
    bool          use_local;
    Glib::ustring remote_host;
    
} ntpg_config;

extern ntpg_config app_config;

void set_default_config();


/**
 * ntpg_page - All 'tab' widgets inherit from this class
 * 
 */
class ntpg_page : public Gtk::Grid
{
  protected:
    Gtk::Label title_lbl;
    Gtk::VSeparator vsep;

  public:
    Glib::ustring Name;
    Glib::ustring Title;

    virtual void do_reload_conf(void) { };
};


class KeyVal : public Gtk::Table
{
  public:
    KeyVal();
    KeyVal(Glib::ustring, Glib::ustring);

    /* void set_key(Glib::ustring); */
    void set(Glib::ustring, Glib::ustring);

  private:
    Gtk::Label key;
    Gtk::Label val;
};

class KeyValTable : public Gtk::Table
{
  public:
    KeyValTable();
    /* KeyValTable(Glib::ustring, Glib::ustring); */

    void add(Glib::ustring, Glib::ustring);

  private:
    std::vector<KeyVal*> elms;
};

std::string trim_wsp(std::string &);
std::vector<Glib::ustring> *get_file_lines(Glib::ustring &);
bool is_ntp_conf_keyword(Glib::ustring);

#endif
