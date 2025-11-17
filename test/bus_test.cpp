/* -*- Mode: c++; c-basic-offset: 4; tab-width: 4; coding: utf-8; -*-  */
/*
 * Copyright (C) 2025 RPf
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdlib.h>          // for exit()
#include <dbus/dbus.h>       // for dbus_*
#include <dbus/dbus-glib.h>  // for dbus_g_*
#include <glibmm.h>
#include <giomm.h>

#include "bus_test.hpp"

bus_test::bus_test()
{

}

// these are not tests, just some consideration about how is it done

static bool
cTest()
{
  // g_type_init ();    deprecated
    std::cout << "CTest ----------------------" << std::endl;

  GError* error = NULL;
  DBusGConnection* connection = dbus_g_bus_get (DBUS_BUS_SESSION,
                           &error);
  if (connection == NULL) {
      g_printerr ("Failed to open connection to bus: %s\n",
              error->message);
      g_error_free (error);
      return false;
    }

  /* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */

  DBusGProxy* proxy = dbus_g_proxy_new_for_name (connection,
                                 DBUS_SERVICE_DBUS,
                                 DBUS_PATH_DBUS,
                                 DBUS_INTERFACE_DBUS);

  /* Call ListNames method, wait for reply */
  error = NULL;
  char **name_list;
  if (!dbus_g_proxy_call (proxy, "ListNames", &error, G_TYPE_INVALID,
                      G_TYPE_STRV, &name_list, G_TYPE_INVALID)) {
      /* Just do demonstrate remote exceptions versus regular GError */
      if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION) {
        g_printerr ("Caught remote method exception %s: %s",
            dbus_g_error_get_name (error), error->message);
        g_error_free (error);
        return false;
      }
      else {
        g_printerr ("Error: %s\n", error->message);
        g_error_free (error);
        return false;
      }
    }

  /* Print the results */

  g_print ("Names on the message bus:\n");

  char **name_list_ptr;
  for (name_list_ptr = name_list; *name_list_ptr; name_list_ptr++) {
      g_print ("  %s\n", *name_list_ptr);
    }
  g_strfreev (name_list);
  g_object_unref (proxy);
  return true;
}


static bool
cppTest()
{
    std::cout << "CppTest ----------------------" << std::endl;
    try {
        auto connection = Gio::DBus::Connection::get_sync (Gio::DBus::BusType::BUS_TYPE_SESSION);
        if (!connection) {  // it will more likely throw
            g_printerr ("Failed to open connection to bus:\n");
            return false;
        }
        /* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */
        auto proxy = Gio::DBus::Proxy::create_sync (connection,
                                     DBUS_SERVICE_DBUS,
                                     DBUS_PATH_DBUS,
                                     DBUS_INTERFACE_DBUS);
        /* Call ListNames method, wait for reply */
        Glib::Variant<std::vector<Glib::VariantBase>> param;
        Glib::VariantContainerBase ret = proxy->call_sync ("ListNames", param);
        if (!ret) {// it will more likely throw
            g_printerr ("Error: ListNames\n");
          return false;
        }

        std::cout << "Names on the message bus:" << std::endl;
                //<< ret.get_type_string()
                //<< " n " << ret.get_n_children() << std::endl;

        Glib::Variant<std::vector<Glib::ustring>> arr;
        ret.get_child(arr, 0);
        //std::cout << "arr " << arr.get_type_string()
        //          << " n " << arr.get_n_children() << std::endl;

        for (uint i = 0; i < arr.get_n_children(); i++) {
            Glib::ustring item = arr.get_child(i);
            std::cout << "  i " << i << " " << item /*<< " type " << strs[i].get_type_string()*/ << std::endl;
        }
        return true;
    }
    catch (const Glib::Exception& ex) {
        std::cout << "Error " << ex.what() << std::endl;
    }
    return false;
}


static bool
cppIntro()
{
    std::cout << "CppIntro ----------------------" << std::endl;
    try {
        //auto connection = Gio::DBus::Connection::get_sync (Gio::DBus::BusType::BUS_TYPE_SESSION);
        //if (!connection) {  // it will more likely throw
        //    g_printerr ("Failed to open connection to bus:\n");
        //    return false;
        //}
        /* Create a proxy object for the "bus driver" (name "org.freedesktop.DBus") */
        //auto proxy = Gio::DBus::Proxy::create_sync (connection,
        //                             DBUS_SERVICE_DBUS,
        //                             "/org/freedesktop/thumbnails/Thumbnailer1",
        //                             "org.freedesktop.thumbnails.Thumbnailer1");

//
//        auto proxy = Gio::DBus::Proxy::create_for_bus_sync(Gio::DBus::BUS_TYPE_SESSION,
//            "org.freedesktop.thumbnails.Thumbnailer1",
//            "/org/freedesktop/thumbnails/Thumbnailer1",
//            "org.freedesktop.thumbnails.Thumbnailer1",
//            Glib::RefPtr<Gio::DBus::InterfaceInfo>(),
//            Gio::DBus::PROXY_FLAGS_NONE);
//
//        std::cout << "query" << std::endl;
//        /* Call ListNames method, wait for reply */
//        Glib::Variant<std::vector<Glib::VariantBase>> param;
//        Glib::VariantContainerBase ret = proxy->call_sync ("Introspect", param);
//        if (!ret) {// it will more likely throw
//            g_printerr("Error: ListIntf\n");
//            return false;
//        }
//
//        std::cout << "Interface:"
//                << ret.get_type_string()
//                << " n " << ret.get_n_children() << std::endl;
//
//        Glib::Variant<Glib::ustring> arg;
//        ret.get_child(arg, 0);
//        //std::cout << "arr " << arr.get_type_string()
//        //          << " n " << arr.get_n_children() << std::endl;
//
//        std::cout << arg.get() /*<< " type " << strs[i].get_type_string()*/ << std::endl;
//        //for (uint i = 0; i < arr.get_n_children(); i++) {
//        //    Glib::ustring item = arr.get_child(i);
//        //}
        return true;
    }
    catch (const Glib::Exception& ex) {
        std::cout << "Error " << ex.what() << std::endl;
    }
    return false;
}


int
main(int argc, char** argv)
{
    Gio::init();    // includes Glib::init()
    if (!cTest()) {
        return 1;
    }
    if (!cppTest()) {
        return 2;
    }
    if (!cppIntro()) {
        return 3;
    }

    return 0;
}