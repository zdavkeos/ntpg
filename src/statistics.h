/**
 * @file   ntpg_statistics.h
 * @author Zach Davis
 * @date   Sep 08, 2011
 * 
 * @brief  widget to display stat files
 * 
 * 
 */


#ifndef NTPG_STATS_H
#define NTPG_STATS_H

#include <gtkmm.h>

#include "ntp_interface.h"
#include "statistics.h"

class ntpg_StatGraph : public Gtk::DrawingArea
{
  public:
    ntpg_StatGraph();
    
    void set_data();
    void process();
    gint save_graph_to_png(const std::string &);

  protected:
    virtual bool on_expose_event(GdkEventExpose*);
    void draw_border();

  private:
    Cairo::RefPtr<Cairo::Context> cr;
    int width;
    int height;
};


class ntpg_Stats : public Gtk::Table
{
  public:
    ntpg_Stats();
    
    Glib::ustring *ntpd_statsdir;
  protected:
    Gtk::Label lbl;
    Gtk::HSeparator sep;
    Gtk::FileChooserWidget *file_chooser;
    Gtk::Button read_button;
    Gtk::Button save_button;
    ntpg_StatGraph *graph;
    Gtk::Label stats_lbl;

    void read_cb(void);
    void save_cb(void);
};

#endif
