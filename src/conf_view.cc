/**
 * @file   ntpg_conf_view.cc
 * @author Zach Davis
 * @date   Sep 01, 2011
 * 
 * @brief  ntp.conf viewer
 * 
 * 
 */

#include "conf_view.h"

#include <iostream>

ntpg_ConfView::ntpg_ConfView()
{
    Name = "Configuration";
    Title = "NTP Configuration File Viewer";
    set_border_width(10);
    sv.set_editable(false);
    Pango::FontDescription fdesc(app_config.file_font);
    sv.modify_font(fdesc);
    // sv.override_font(fdesc); // gtkmm3.0

    tbuff_ref = sv.get_buffer();
    comment_tag = tbuff_ref->create_tag("comment");
    comment_tag->property_foreground() = "DarkGreen";
    keyword_tag = tbuff_ref->create_tag("keyword");
    // keyword_tag->property_foreground() = "Red";
    keyword_tag->property_font() = "bold";

    // set priority of tags
    keyword_tag->set_priority(0);
    comment_tag->set_priority(1);
    

    cb_reload(); // get the file contents

    lbl.set_text("NTP Configuration File: " + ntp_conf_file);
    sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    edit_button = new Gtk::Button(Gtk::Stock::EDIT);
    reload_button = new Gtk::Button(Gtk::Stock::REFRESH);
    
    edit_button->signal_clicked().connect(sigc::mem_fun(*this,
                                         &ntpg_ConfView::cb_edit));
    reload_button->signal_clicked().connect(sigc::mem_fun(*this,
                                         &ntpg_ConfView::cb_reload));
    
    // pack everything together
    sw.add(sv);
    attach(lbl, 0, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);
    attach(sep, 0, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    attach(sw, 0, 2, 2, 3);
    attach(*edit_button, 0, 1, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    attach(*reload_button, 0, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
}

ntpg_ConfView::~ntpg_ConfView()
{
    delete edit_button;
    delete reload_button;
}

void ntpg_ConfView::do_reload_conf()
{
    ntp_conf_file = app_config.default_conf_file;
    editor = app_config.editor;
    Pango::FontDescription fdesc(app_config.file_font);
    sv.modify_font(fdesc);
}

void ntpg_ConfView::reload_file(void)
{
    do_reload_conf();

    int ret = ntpd_interface::read_ntp_conf(ntp_conf_file, buff_txt);
    if(ret != 0) {
        std::cout << "Unable to read ntp.conf file." << std::endl;
        buff_txt.clear();
        buff_txt.append("\tUnable to read: ");
        buff_txt.append(ntp_conf_file);
    }

    tbuff_ref->set_text(buff_txt);
}

void ntpg_ConfView::markup()
{
    // gint lines = tbuff_ref->get_line_count();
    Gtk::TextIter a, b;

    // keywords
    b = tbuff_ref->begin();
    
    while (b.forward_word_end()) {
        a = b;
        a.backward_word_start();

        // check if we are at the start of the line?
        if(is_ntp_conf_keyword(a.get_slice(b)))
           tbuff_ref->apply_tag_by_name("keyword", a, b);
    }

    // comments last
    for (a = tbuff_ref->begin(); a != tbuff_ref->end(); a++) {
        if (a.get_char() == '#') {
            b = a;
            b.forward_line();

            tbuff_ref->apply_tag_by_name("comment", a, b--);
        }
    }
}

void ntpg_ConfView::cb_edit()
{
    Glib::ustring cmd = editor;
    cmd.append(" ");
    cmd += ntp_conf_file;

    Glib::spawn_command_line_async(cmd);
}

void ntpg_ConfView::cb_reload()
{
    reload_file();
    markup();
}
