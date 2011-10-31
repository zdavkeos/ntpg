/**
 * @file   ntpg_ui.h
 * @author Zach Davis
 * @date   Aug 31, 2011
 * 
 * @brief  
 * 
 * 
 */

#ifndef NTPG_UI_H
#define NTPG_UI_H

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include <gtkmm.h>

#include "conf_view.h"
#include "summary.h"
#include "peers.h"
#include "statistics.h"
#include "util.h"


class ntpg_config_ui : public Gtk::Dialog
{
  public:
    ntpg_config_ui();
    ~ntpg_config_ui();
    void do_run(void);

  private:
    Gtk::Notebook book;
    // Gtk::FontChooserWidget fcw;
    Gtk::Table props;
    Gtk::CheckButton *use_dns_btn;
    Gtk::Label font_lbl;
    Gtk::FontButton *font_chooser;
    Gtk::Entry *editor_entry;
    Gtk::Label editor_lbl;
    Gtk::Entry *conf_entry;
    Gtk::Label conf_lbl;

    void apply_cb(void);
    void cancle_cb(void) { hide(); }
    
};

class ntpg_main : public Gtk::Window
{
  public:
    ntpg_main();
    // ~ntpg_main();

    void set_status_message(guint);
    // std::map<Glib::ustring, guint> Messages;

  protected:
    // child widgets
    Gtk::VBox main_vbox;
    Gtk::Notebook main_notebookref;
    ntpg_ConfView conf_viewer;
    ntpg_summary ntpsumm;
    ntpg_peers   peers;
    ntpg_Stats   stats;
    Gtk::Statusbar statusbar;
    ntpg_config_ui *conf_dlg;
    
    Glib::RefPtr<Gtk::UIManager>   main_UIManager_ref;
    Glib::RefPtr<Gtk::ActionGroup> main_ActionGroup_ref;

    // callbacks
    void on_menu_file_quit();
    void on_menu_help_about();
    void on_menu_config();
    void on_menu_generic();
};

#endif
