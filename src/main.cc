/**
 * @file   main.cc
 * @author Zach Davis
 * @date   Aug 31, 2011
 * 
 * @brief  Application entry point
 * 
 * 
 */

#include <gtkmm.h>

#include "ui.h"
#include "util.h"
#include "ntp_interface.h"

int main(int argc, char **argv)
{
    Glib::ustring ver_str;

    Gtk::Main gtk(argc, argv);

    // check to see if ntpd command exists...
    if(!ntpd_interface::ntpd_get_version(ver_str))
    {
        Gtk::MessageDialog dlg("<big>Unable to run ntpd command!</big>"
                               "\n\n Make sure you have ntp installed",
                               TRUE,
                               Gtk::MESSAGE_ERROR,
                               Gtk::BUTTONS_CLOSE);
        dlg.run();
        return -1;
    }

    set_default_config();
    ntpg_main ntpg_window;

    Gtk::Main::run(ntpg_window);

    return 0;
}
