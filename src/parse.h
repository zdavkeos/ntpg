/**
 * @file   ntpg_parse.h
 * @author Zach Davis
 * @date   Sep 13, 2011
 * 
 * @brief  Parse ntp statistics files
 * 
 * 
 */

#ifndef NTPG_PARSE_H
#define NTPG_PARSE_H

#include <string>
#include <vector>

#include <gtkmm.h>

#include "statistics.h"

class Stats
{
  public:
    virtual gint parse() = 0;
    virtual Glib::ustring stats_string() = 0;
    virtual gdouble min() = 0;
    virtual gdouble max() = 0;
    virtual ntpg_StatGraph* get_graph() = 0;
};


struct LoopStatsLine
{
    guint   day;
    gdouble second;
    gdouble offset;
    gdouble drift_comp;
    gdouble error;
    gdouble stability;
    guint   poll_interval;
};

class LoopStats : public Stats
{
  public:
    LoopStats(Glib::ustring &);
    
    gint parse();
    Glib::ustring stats_string();
    gdouble min();
    gdouble max();
    ntpg_StatGraph* get_graph();
  private:
    Glib::ustring file;
    std::vector<LoopStatsLine*> stats;
    void calc_averages();
    gdouble ave_offset;
    gdouble ave_drift;
    gdouble ave_error;
    gdouble ave_stability;
    gdouble ave_interval;
};

#endif
