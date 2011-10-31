/**
 * @file   ntpg_ui.cc
 * @author Zach Davis
 * @date   Aug 30, 2011
 * 
 * @brief  builds the main window
 * 
 * 
 */

#include <iostream>

#include "ui.h"
#include "util.h"

ntpg_main::ntpg_main()
{
    set_title("Gtk NTP monitor");
    set_default_size(850, 400);
    set_default_config(); // apply default settings
    conf_dlg = NULL;

    add(main_vbox);

    // Create actions for menus and toolbars
    main_ActionGroup_ref = Gtk::ActionGroup::create();

    // File menu:
    main_ActionGroup_ref->add(Gtk::Action::create("FileMenu", "File"));
    main_ActionGroup_ref->add(Gtk::Action::create("FilePrefs", Gtk::Stock::PROPERTIES),
                              sigc::mem_fun(*this, &ntpg_main::on_menu_config));
    main_ActionGroup_ref->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                              sigc::mem_fun(*this, &ntpg_main::on_menu_file_quit));

    // Help menu
    main_ActionGroup_ref->add(Gtk::Action::create("HelpMenu", "Help") );
    main_ActionGroup_ref->add(Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
                              sigc::mem_fun(*this, &ntpg_main::on_menu_help_about) );

    main_UIManager_ref = Gtk::UIManager::create();
    main_UIManager_ref->insert_action_group(main_ActionGroup_ref);

    add_accel_group(main_UIManager_ref->get_accel_group());

    // Layout the actions in a menubar and toolbar
    Glib::ustring main_ui_def = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FilePrefs' />"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "    <toolitem action='FilePrefs'/>"
        "    <toolitem action='FileQuit'/>"
        "    <separator/>"
        "    <toolitem action='HelpAbout'/>"
        "  </toolbar>"
        "</ui>";

    try
    {
        main_UIManager_ref->add_ui_from_string(main_ui_def);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "Building menus failed: " <<  ex.what();
    }

    // Get the menubar and toolbar widgets, and add them to a container widget
    Gtk::Widget* pMenubar = main_UIManager_ref->get_widget("/MenuBar");
    if(pMenubar)
        main_vbox.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    Gtk::Widget* pToolbar = main_UIManager_ref->get_widget("/ToolBar") ;
    if(pToolbar)
        main_vbox.pack_start(*pToolbar, Gtk::PACK_SHRINK);

    // add the main widgets
    main_vbox.pack_start(main_notebookref, Gtk::PACK_EXPAND_WIDGET, 5);
    
    // tabs
    // main_notebookref.append_page();
    main_notebookref.append_page(ntpsumm, ntpsumm.Name);
    main_notebookref.append_page(peers, peers.Name);
    main_notebookref.append_page(conf_viewer, conf_viewer.Name);
    main_notebookref.append_page(stats, "Statistics");
    

    // finally the status bar...
    main_vbox.pack_start(statusbar, Gtk::PACK_SHRINK);

    show_all_children();
}

void ntpg_main::on_menu_file_quit()
{
    // 'hide' is misleading... it actually quits
    hide();
}

void ntpg_main::on_menu_help_about()
{
    Gtk::AboutDialog dlg;
    dlg.set_program_name(PACKAGE_NAME);
    dlg.set_version(VERSION);
    dlg.set_license("GPLv3");
	// dlg.set_logo(0);
    dlg.set_comments("GTK+ NTP daemon monitoring program");

    std::vector<Glib::ustring> authors;
    authors.push_back("Zach Davis");
    dlg.set_authors(authors);
    
    dlg.run();
    dlg.hide();
}

void ntpg_main::on_menu_generic()
{
    std::cout << "A menu item was selected." << std::endl;
}

void ntpg_main::on_menu_config()
{
    if(conf_dlg == NULL) {
        conf_dlg = new ntpg_config_ui();
    }

    conf_dlg->do_run();
}

ntpg_config_ui::ntpg_config_ui()
{
    add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_ACCEPT);
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

    // build properties page
    use_dns_btn = new Gtk::CheckButton("Use DNS lookup");
    use_dns_btn->set_active(app_config.use_dns);
    font_lbl.set_text("Config File Font");
    font_chooser = new Gtk::FontButton(app_config.file_font);
    editor_lbl.set_text("Editor");
    editor_entry = new Gtk::Entry();
    editor_entry->set_text(app_config.editor);
    conf_lbl.set_text("NTP Config File");
    conf_entry = new Gtk::Entry();
    conf_entry->set_text(app_config.default_conf_file);
    

    props.attach(*use_dns_btn, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(font_lbl, 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(*font_chooser, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(editor_lbl, 0, 1, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(*editor_entry, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(conf_lbl, 0, 1, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    props.attach(*conf_entry, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK, 5, 5);

    // book.append_page("Font", fcw);
    book.append_page(props, "Properties");

    get_vbox()->pack_start(book);

    show_all();
}

ntpg_config_ui::~ntpg_config_ui()
{
    
}

void ntpg_config_ui::apply_cb()
{
    app_config.use_dns = use_dns_btn->get_active();
    app_config.file_font = font_chooser->get_font_name();
    app_config.editor = editor_entry->get_text();
    app_config.default_conf_file = conf_entry->get_text();
}

void ntpg_config_ui::do_run()
{
    int ret;

    ret = run();

    if(ret == Gtk::RESPONSE_ACCEPT) {
        apply_cb();
    } else if(ret == Gtk::RESPONSE_CANCEL) {
        // pass
    }

    hide();
}
