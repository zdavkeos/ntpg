/**
 * @file   ntpg_statistics.cc
 * @author Zach Davis
 * @date   Sep 08, 2011
 * 
 * @brief  stats files display
 * 
 */


#include "statistics.h"

#include <iostream>

ntpg_Stats::ntpg_Stats()
    : Gtk::Table(3, 2, false)
{
    ntpd_statsdir = new Glib::ustring("/var/log/ntpstats");
    file_chooser = new Gtk::FileChooserWidget(Gtk::FILE_CHOOSER_ACTION_OPEN);
    graph = new ntpg_StatGraph();

    set_border_width(6);
    read_button.set_label("Open File");
    save_button.set_label("Save Graph");
    file_chooser->set_select_multiple(false);
    file_chooser->set_create_folders(false);
    file_chooser->set_current_folder("/var/log/ntpstats");

    read_button.signal_clicked().connect(
        sigc::mem_fun(this, &ntpg_Stats::read_cb));
    save_button.signal_clicked().connect(
        sigc::mem_fun(this, &ntpg_Stats::save_cb));

    lbl.set_text("NTP statistics files - " + *ntpd_statsdir);
    stats_lbl.set_text("Graph statistics");

    add(lbl);
    attach(sep, 0, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    attach(*file_chooser, 0, 1, 2, 3);
    attach(read_button, 0, 1, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    attach(save_button, 1, 2, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    attach(*graph, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 3, 3);
    attach(stats_lbl, 1, 2, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);


    // attach(lbl, 0, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);
    // attach(sep, 0, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    // attach(*file_chooser, 0, 1, 2, 3);
    // attach(read_button, 0, 1, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    // attach(save_button, 1, 2, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    // attach(*graph, 1, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 3, 3);
    // attach(stats_lbl, 1, 2, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);

    graph->show();
}

void ntpg_Stats::read_cb(void)
{
    std::cout << "read button clicked: " << file_chooser->get_filename() << std::endl;
}

void ntpg_Stats::save_cb(void)
{
    gint response;
    std::string fname;
    Gtk::FileChooserDialog chooser("Save graph to PNG file", Gtk::FILE_CHOOSER_ACTION_SAVE);
    Glib::RefPtr<Gtk::FileFilter> png_filt;
    
    png_filt = Gtk::FileFilter::create();
    chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_ACCEPT);
    png_filt->set_name("PNG Files");
    png_filt->add_mime_type("image/png");
    png_filt->add_pattern("*.png");
    chooser.set_filter(png_filt);

    response = chooser.run();

    if(response == Gtk::RESPONSE_ACCEPT) {
        fname = chooser.get_filename();
        graph->save_graph_to_png(fname);
    } else {
        // user cancel
        return;
    }

}

ntpg_StatGraph::ntpg_StatGraph()
{

}

gint ntpg_StatGraph::save_graph_to_png(const std::string &fname)
{
    try {
        Glib::RefPtr<Gdk::Window> wnd = get_window();
        Cairo::RefPtr<Cairo::Context> ctx = wnd->create_cairo_context();
        Cairo::RefPtr<Cairo::Surface> srf = ctx->get_target();
        srf->write_to_png(fname);
    } catch(...) {
        // error of some sort? better safe than sorry
        return -1;
    }

    return 0;
}

bool ntpg_StatGraph::on_expose_event(GdkEventExpose* event)
{
    // This is where we draw on the window
    Glib::RefPtr<Gdk::Window> window = get_window();
    if(window)
    {
        Gtk::Allocation allocation = get_allocation();
        width = allocation.get_width();
        height = allocation.get_height();

        cr = window->create_cairo_context();

        // clip to the area indicated by the expose event so that we only redraw
        // the portion of the window that needs to be redrawn
        cr->rectangle(event->area.x, event->area.y,
                      event->area.width, event->area.height);
        cr->clip();

        draw_border();
    }

    return true;
}

void ntpg_StatGraph::draw_border()
{
    // border
    cr->set_line_width(10.0);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->move_to(0, 0);
    cr->line_to(width, 0);
    cr->line_to(width, height);
    cr->line_to(0, height);
    cr->line_to(0, 0);
    cr->stroke();

    // center line
    cr->set_line_width(1.0);
    cr->set_source_rgb(0.0, 0.0, 0.0);
    std::vector<double> dashes;
    dashes.push_back(3.0);
    cr->set_dash(dashes, 0.0);
    cr->move_to(0, height / 2);
    cr->line_to(width, height / 2);
    cr->stroke();
}
