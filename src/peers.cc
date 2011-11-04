/**
 * @file   ntpg_peers.cc
 * @author Zach Davis
 * @date   Aug 30, 2011
 * 
 * @brief  grab ntpq peers list
 * 
 * 
 */

#include <sstream>
#include <vector>

#include "peers.h"
#include "ntp_interface.h"

Peer::Peer()
{

}

void Peer::from_string(std::string &s)
{
    std::stringstream ss(s);

    ss >> remote;
    if(remote[0] == 'o')
        tally = "PPS";
    else if(remote[0] == '+')
        tally = "good";
    else if(remote[0] == '*')
        tally = "used";
    else if(remote[0] == '-')
        tally = "bad";
    else
        tally = "";
    ss >> refid;
    ss >> st;
    ss >> t;
    if(t == "l")
        t = "local";
    else if(t == "u")
        t = "unicast";
    else if(t == "b")
        t = "broadcast";
    ss >> when;
    ss >> poll;
    ss >> reach;
    ss >> delay;
    ss >> offset;
    ss >> jitter;
}

void ntpg_peers::update()
{

    /* Glib::Thread *const worker = */
    Glib::Thread::create(
        sigc::mem_fun(*this, &ntpg_peers::update_worker), false);
}

void ntpg_peers::update_worker()
{
    Glib::Mutex::Lock lock(upd_mtx);

    std::vector<std::string> plist;
    std::vector<std::string>::iterator it;
    Gtk::TreeModel::iterator tmit;

    progress.set_fraction(0.0);
    progress.set_text("Polling ntpd...");

    progress.pulse();
    ntpd_interface::get_peers_list(plist);
    progress.pulse();

    progress.set_fraction(0.9);
    list->clear();
    for(it = plist.begin(); it != plist.end(); it++)
    {
        Peer p;
        p.from_string(*it);
        
        tmit = list->append();
        Gtk::TreeModel::Row row = *tmit;
 
        row[peercols.m_remote] = p.remote;
        row[peercols.m_refid]  = p.refid;
        row[peercols.m_st]     = p.st;
        row[peercols.m_t]      = p.t;
        row[peercols.m_when]   = p.when;
        row[peercols.m_poll]   = p.poll;
        row[peercols.m_reach]  = p.reach;
        row[peercols.m_delay]  = p.delay;
        row[peercols.m_offset] = p.offset;
        row[peercols.m_jitter] = p.jitter;

        if(p.tally == "PPS") {
            // color row
            // tview.get_cell_area(row, peercols.m_remote, 
            
        }
    }

    progress.set_fraction(1.0);
    progress.set_text("Done");
}

ntpg_peers::ntpg_peers()
{
    Name = "Peers";
    Title = "List of NTP peers";

    list = Gtk::ListStore::create(peercols);
    legend = NULL;
    leg_butt.set_label("Legend");
    leg_butt.signal_clicked().connect(
        sigc::mem_fun(this, &ntpg_peers::leg_butt_clicked));

    // setup the view
    tview.set_model(list);
    tview.set_headers_visible();
    tview.append_column("Remote", peercols.m_remote);
    tview.append_column("Ref ID", peercols.m_refid);
    tview.append_column("Stratum", peercols.m_st);
    tview.append_column("Transmit", peercols.m_t);
    tview.append_column("Poll", peercols.m_when);
    tview.append_column("Poll Interval", peercols.m_poll);
    tview.append_column("Reach", peercols.m_reach);
    tview.append_column_numeric("Delay", peercols.m_delay, "%03.03f");
    tview.append_column_numeric("Offset", peercols.m_offset, "%03.03f");
    tview.append_column_numeric("Jitter", peercols.m_jitter, "%03.03f");

    update(); // load the first set of data
    tview.columns_autosize();

    lbl.set_text("Peers List");
    sw.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    // progress.set_show_text(true);
    pollb.set_label("Poll");
    pollb.signal_clicked().connect(sigc::mem_fun(*this,
                                                 &ntpg_peers::update));
    
    sw.add(tview);
    attach(lbl, 0, 3, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);
    attach(sep, 0, 3, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    attach(sw, 0, 3, 2, 3);
    attach(pollb, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    attach(leg_butt, 0, 1, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    attach(progress, 2, 3, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 5, 5);

}

void ntpg_peers::do_reload_conf()
{
    
}

void ntpg_peers::leg_butt_clicked()
{
    if(legend == NULL) {
        legend = new Legend();
    }

    legend->run();
    legend->hide();
}

PeersColumns::PeersColumns()
{
    // create the actual columns
    add(m_remote);
    add(m_refid);
    add(m_st);
    add(m_t);
    add(m_when);
    add(m_poll);
    add(m_reach);
    add(m_delay);
    add(m_offset);
    add(m_jitter);
}

Legend::Legend()
    : Gtk::Dialog("NTP Peer Legend", false)
{
    Gtk::ButtonBox* bbox = get_action_area();
    bbox->set_layout(Gtk::BUTTONBOX_EDGE);
    Gtk::Box* vbox = get_vbox();
    moreb.set_uri("http://ntp.org");
    moreb.set_label("NTP website");
    bbox->pack_start(moreb);
    /*Gtk::Button *close = */add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_DELETE_EVENT);
    // close->signal_clicked

    // fill out the labels
    leg_lbl.set_text("Legend");
    tally_lbl.set_text("Tally Mark");
    flds_lbl.set_text("Fields");

    pps.set("'o' - ", "PPS");
    pre.set("'*' - ", "System peer");
    pft.set("'X' - ", "Falseticker");
    pex.set("'.' - ", "Excess");
    pol.set("'-' - ", "Outlier");
    pca.set("'+' - ", "Candidate");
    pse.set("'#' - ", "Selected");
    
    // pack it all together
    tbl.attach(leg_lbl, 0, 5, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);
    tbl.attach(hsep, 0, 5, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    tbl.attach(tally_lbl, 0, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);
    tbl.attach(vsep, 2, 3, 2, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 3, 3);
    tbl.attach(flds_lbl, 3, 5, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 10, 10);

    tbl.attach(pps, 0, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pre, 0, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pft, 0, 2, 5, 6, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pex, 0, 2, 6, 7, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pol, 0, 2, 7, 8, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pca, 0, 2, 8, 9, Gtk::SHRINK, Gtk::SHRINK, 3, 3);
    tbl.attach(pse, 0, 2, 9, 10,Gtk::SHRINK, Gtk::SHRINK, 3, 3);

    vbox->pack_start(tbl);
    vbox->show_all();
}
