/**
 * @file   ntpg_parse.cc
 * @author Zach Davis
 * @date   Sep 13, 2011
 * 
 * @brief  Parse ntpd stats files
 * 
 * 
 */

#include <iostream>
#include <sstream>

#include "parse.h"
#include "util.h"

LoopStats::LoopStats(Glib::ustring &filename) 
    : file(filename)
{
    
}

gint LoopStats::parse()
{
    std::vector<Glib::ustring>::iterator it;
    std::vector<Glib::ustring> *lines = get_file_lines(file);
    struct LoopStatsLine *stat;

    if (lines->size() == 0)
        return -1;

    stats.clear();

    for (it = lines->begin(); it != lines->end(); it++) {
        stat = new struct LoopStatsLine;
        
        try {
            std::stringstream ss(*it);
            
            ss >> stat->day;
            ss >> stat->second;
            ss >> stat->offset;
            ss >> stat->drift_comp;
            ss >> stat->error;
            ss >> stat->stability;
            ss >> stat->poll_interval;
            
            stats.push_back(stat);
        } catch(...) {
            return -1;
        }
    }

    delete lines;

    return 0;
}

gdouble LoopStats::min()
{
    return 0.0;
}

gdouble LoopStats::max()
{
    return 0.0;
}

Glib::ustring LoopStats::stats_string()
{
    Glib::ustring str("Min: ");
    

    return str;
}

void LoopStats::calc_averages()
{
    guint count = 1;
    ave_offset    = 0.0;
    ave_drift     = 0.0;
    ave_error     = 0.0;
    ave_stability = 0.0;
    ave_interval  = 0.0;

    std::vector<LoopStatsLine*>::iterator it;

    for (it = stats.begin(); it != stats.end(); it++, count++) {
        ave_offset    += (*it)->offset;
        ave_drift     += (*it)->drift_comp;
        ave_error     += (*it)->error;
        ave_stability += (*it)->stability;
        ave_interval  += (*it)->poll_interval;
    }

    ave_offset    /= count;
    ave_drift     /= count;
    ave_error     /= count;
    ave_stability /= count;
    ave_interval  /= count;
}

ntpg_StatGraph* LoopStats::get_graph()
{
    return new ntpg_StatGraph();
}
