/**
 * @file   ntpg_conf_view.h
 * @author Zach Davis
 * @date   Sep 01, 2011
 * 
 * @brief  ntp.conf viewer class
 * 
 * 
 */

#ifndef NTPG_CONF_VIEW_H
#define NTPG_CONF_VIEW_H

#include <gtkmm.h>

#include "ntp_interface.h"
#include "util.h"

class ntpg_ConfView : public ntpg_page
{
  public:
    ntpg_ConfView();
    ~ntpg_ConfView();

    void do_reload_conf(void);

    Glib::ustring ntp_conf_file;
    Glib::ustring editor;
  protected:
    Gtk::Label lbl;
    Gtk::ScrolledWindow sw;
    Gtk::TextView sv;
    Glib::RefPtr<Gtk::TextBuffer> tbuff_ref;
    Glib::RefPtr<Gtk::TextTag> comment_tag;
    Glib::RefPtr<Gtk::TextTag> keyword_tag;
    /* Pango::FontDescription fdesc; */

    Glib::ustring buff_txt;
    Gtk::HSeparator sep;
    Gtk::Button *edit_button;
    Gtk::Button *reload_button;

    /* gint read_ntp_conf(gchar *, Glib::ustring &); */
    void reload_file(void);
    void markup();
    void cb_edit();
    void cb_reload();
};

#endif
