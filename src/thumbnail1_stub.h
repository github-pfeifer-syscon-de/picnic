#pragma once
#include <string>
#include <glibmm.h>
#include <giomm.h>
#include "thumbnail1_common.h"

namespace org {
namespace freedesktop {
namespace DBus {

class Properties {
public:
    Properties();
    virtual ~Properties();

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);

    // deprecated:
    void connect(Gio::DBus::BusType, std::string);


protected:
    virtual void Get(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        PropertiesMessageHelper msg) = 0;
    virtual void GetAll(
        const Glib::ustring & interface_name,
        PropertiesMessageHelper msg) = 0;
    virtual void Set(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        const Glib::VariantBase & value,
        PropertiesMessageHelper msg) = 0;

    void PropertiesChanged_emitter(Glib::ustring, std::map<Glib::ustring,Glib::VariantBase>, std::vector<Glib::ustring>);
    sigc::signal<void, Glib::ustring, std::map<Glib::ustring,Glib::VariantBase>, std::vector<Glib::ustring>> PropertiesChanged_signal;

    void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                         const Glib::ustring &name);

    void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &name);

    void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                      const Glib::ustring &name);

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    guint connectionId, registeredId;
    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    Glib::RefPtr<Gio::DBus::Connection> m_connection;
    std::string m_objectPath;
    std::string m_interfaceName;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace DBus {

class Introspectable {
public:
    Introspectable();
    virtual ~Introspectable();

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);

    // deprecated:
    void connect(Gio::DBus::BusType, std::string);


protected:
    virtual void Introspect(
        IntrospectableMessageHelper msg) = 0;

    void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                         const Glib::ustring &name);

    void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &name);

    void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                      const Glib::ustring &name);

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    guint connectionId, registeredId;
    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    Glib::RefPtr<Gio::DBus::Connection> m_connection;
    std::string m_objectPath;
    std::string m_interfaceName;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace DBus {

class Peer {
public:
    Peer();
    virtual ~Peer();

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);

    // deprecated:
    void connect(Gio::DBus::BusType, std::string);


protected:
    virtual void Ping(
        PeerMessageHelper msg) = 0;
    virtual void GetMachineId(
        PeerMessageHelper msg) = 0;

    void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                         const Glib::ustring &name);

    void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &name);

    void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                      const Glib::ustring &name);

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    guint connectionId, registeredId;
    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    Glib::RefPtr<Gio::DBus::Connection> m_connection;
    std::string m_objectPath;
    std::string m_interfaceName;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace thumbnails {

class Thumbnailer1 {
public:
    Thumbnailer1();
    virtual ~Thumbnailer1();

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);

    // deprecated:
    void connect(Gio::DBus::BusType, std::string);


protected:
    virtual void Queue(
        const std::vector<Glib::ustring> & uris,
        const std::vector<Glib::ustring> & mime_types,
        const Glib::ustring & flavor,
        const Glib::ustring & scheduler,
        guint32 handle_to_unqueue,
        Thumbnailer1MessageHelper msg) = 0;
    virtual void Dequeue(
        guint32 handle,
        Thumbnailer1MessageHelper msg) = 0;
    virtual void GetSupported(
        Thumbnailer1MessageHelper msg) = 0;
    virtual void GetSchedulers(
        Thumbnailer1MessageHelper msg) = 0;
    virtual void GetFlavors(
        Thumbnailer1MessageHelper msg) = 0;

    void Started_emitter(guint32);
    sigc::signal<void, guint32> Started_signal;

    void Finished_emitter(guint32);
    sigc::signal<void, guint32> Finished_signal;

    void Ready_emitter(guint32, std::vector<Glib::ustring>);
    sigc::signal<void, guint32, std::vector<Glib::ustring>> Ready_signal;

    void Error_emitter(guint32, std::vector<Glib::ustring>, gint32, Glib::ustring);
    sigc::signal<void, guint32, std::vector<Glib::ustring>, gint32, Glib::ustring> Error_signal;

    void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                         const Glib::ustring &name);

    void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &name);

    void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                      const Glib::ustring &name);

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    guint connectionId, registeredId;
    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    Glib::RefPtr<Gio::DBus::Connection> m_connection;
    std::string m_objectPath;
    std::string m_interfaceName;
};

} // thumbnails
} // freedesktop
} // org

