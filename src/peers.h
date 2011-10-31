/**
 * @file   ntpg_peers.h
 * @author Zach Davis
 * @date   Aug 31, 2011
 * 
 * @brief  ntp peer list stuff
 * 
 * 
 */

#ifndef NTPG_PEER_H
#define NTPG_PEER_H

#include <gtkmm.h>

#include "util.h"

class Legend : public Gtk::Dialog
{
  public:
    Legend();
    // void run();

  private:
    Gtk::Table tbl;
    Gtk::Label leg_lbl;
    Gtk::HSeparator hsep;
    Gtk::VSeparator vsep;
    Gtk::Label tally_lbl;
    Gtk::Label flds_lbl;
    Gtk::LinkButton moreb; 
    
    KeyVal pps;
    KeyVal pre;
};

class Peer
{
  public:
    Glib::ustring remote;
    Glib::ustring refid;
    guint st;
    Glib::ustring t;
    Glib::ustring when;
    guint poll;
    guint reach;
    gfloat delay;
    gfloat offset;
    gfloat jitter;

    Glib::ustring tally;

    Peer();

    void from_string(std::string &);
};

class PeersColumns : public Gtk::TreeModelColumnRecord
{
  public:
    PeersColumns();

    Gtk::TreeModelColumn<Glib::ustring> m_remote;
    Gtk::TreeModelColumn<Glib::ustring> m_refid;
    Gtk::TreeModelColumn<guint>         m_st;
    Gtk::TreeModelColumn<Glib::ustring> m_t;
    Gtk::TreeModelColumn<Glib::ustring> m_when;
    Gtk::TreeModelColumn<guint>         m_poll;
    Gtk::TreeModelColumn<guint>         m_reach;
    Gtk::TreeModelColumn<gfloat>        m_delay;
    Gtk::TreeModelColumn<gfloat>        m_offset;
    Gtk::TreeModelColumn<gfloat>        m_jitter;
};

class ntpg_peers : public ntpg_page
{
  public:
    ntpg_peers();
    void update();
    void do_reload_conf();

  protected:
    void update_worker();

    Gtk::Label lbl;
    Gtk::ScrolledWindow sw;
    Gtk::TreeView tview;
    Glib::RefPtr<Gtk::ListStore> list;
    PeersColumns peercols;
    Gtk::Button pollb;
    Gtk::ProgressBar progress;
    Gtk::HSeparator sep;
    Gtk::Button leg_butt;
    Legend *legend;
    void leg_butt_clicked();
    Glib::Mutex upd_mtx;

};

#endif
