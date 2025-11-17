#pragma once
#include <string>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include "thumbnail1_common.h"

namespace org {
namespace freedesktop {
namespace DBus {

class PropertiesStub : public sigc::trackable {
public:
    PropertiesStub();
    virtual ~PropertiesStub();

    PropertiesStub(const PropertiesStub &other) = delete;
    PropertiesStub(PropertiesStub &&other) = delete;
    PropertiesStub &operator=(const PropertiesStub &other) = delete;
    PropertiesStub &operator=(PropertiesStub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void Get(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        MethodInvocation &invocation) = 0;
    virtual void GetAll(
        const Glib::ustring & interface_name,
        MethodInvocation &invocation) = 0;
    virtual void Set(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        const Glib::VariantBase & value,
        MethodInvocation &invocation) = 0;

    void PropertiesChanged_emitter(const std::vector<Glib::ustring> &destination_bus_names, const Glib::ustring &, const std::map<Glib::ustring,Glib::VariantBase> &, const std::vector<Glib::ustring> &);
    sigc::signal<void, const Glib::ustring &, const std::map<Glib::ustring,Glib::VariantBase> &, const std::vector<Glib::ustring> &> PropertiesChanged_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, const Glib::ustring &, const std::map<Glib::ustring,Glib::VariantBase> &, const std::vector<Glib::ustring> &> PropertiesChanged_selectiveSignal;

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

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class PropertiesStub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret(const Glib::VariantBase & p0) {
        std::vector<Glib::VariantBase> vlist;
        vlist.push_back(p0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret(const std::map<Glib::ustring,Glib::VariantBase> & p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<std::map<Glib::ustring,Glib::VariantBase>> var0 =
            Glib::Variant<std::map<Glib::ustring,Glib::VariantBase>>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret() {
        std::vector<Glib::VariantBase> vlist;

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace DBus {

class IntrospectableStub : public sigc::trackable {
public:
    IntrospectableStub();
    virtual ~IntrospectableStub();

    IntrospectableStub(const IntrospectableStub &other) = delete;
    IntrospectableStub(IntrospectableStub &&other) = delete;
    IntrospectableStub &operator=(const IntrospectableStub &other) = delete;
    IntrospectableStub &operator=(IntrospectableStub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void Introspect(
        MethodInvocation &invocation) = 0;

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

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class IntrospectableStub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret(const Glib::ustring & p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<Glib::ustring> var0 =
            Glib::Variant<Glib::ustring>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace DBus {

class PeerStub : public sigc::trackable {
public:
    PeerStub();
    virtual ~PeerStub();

    PeerStub(const PeerStub &other) = delete;
    PeerStub(PeerStub &&other) = delete;
    PeerStub &operator=(const PeerStub &other) = delete;
    PeerStub &operator=(PeerStub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void Ping(
        MethodInvocation &invocation) = 0;
    virtual void GetMachineId(
        MethodInvocation &invocation) = 0;

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

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class PeerStub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret() {
        std::vector<Glib::VariantBase> vlist;

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret(const Glib::ustring & p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<Glib::ustring> var0 =
            Glib::Variant<Glib::ustring>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // DBus
} // freedesktop
} // org
namespace org {
namespace freedesktop {
namespace thumbnails {

class Thumbnailer1Stub : public sigc::trackable {
public:
    Thumbnailer1Stub();
    virtual ~Thumbnailer1Stub();

    Thumbnailer1Stub(const Thumbnailer1Stub &other) = delete;
    Thumbnailer1Stub(Thumbnailer1Stub &&other) = delete;
    Thumbnailer1Stub &operator=(const Thumbnailer1Stub &other) = delete;
    Thumbnailer1Stub &operator=(Thumbnailer1Stub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void Queue(
        const std::vector<Glib::ustring> & uris,
        const std::vector<Glib::ustring> & mime_types,
        const Glib::ustring & flavor,
        const Glib::ustring & scheduler,
        guint32 handle_to_unqueue,
        MethodInvocation &invocation) = 0;
    virtual void Dequeue(
        guint32 handle,
        MethodInvocation &invocation) = 0;
    virtual void GetSupported(
        MethodInvocation &invocation) = 0;
    virtual void GetSchedulers(
        MethodInvocation &invocation) = 0;
    virtual void GetFlavors(
        MethodInvocation &invocation) = 0;

    void Started_emitter(const std::vector<Glib::ustring> &destination_bus_names, guint32);
    sigc::signal<void, guint32> Started_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, guint32> Started_selectiveSignal;

    void Finished_emitter(const std::vector<Glib::ustring> &destination_bus_names, guint32);
    sigc::signal<void, guint32> Finished_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, guint32> Finished_selectiveSignal;

    void Ready_emitter(const std::vector<Glib::ustring> &destination_bus_names, guint32, const std::vector<Glib::ustring> &);
    sigc::signal<void, guint32, const std::vector<Glib::ustring> &> Ready_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, guint32, const std::vector<Glib::ustring> &> Ready_selectiveSignal;

    void Error_emitter(const std::vector<Glib::ustring> &destination_bus_names, guint32, const std::vector<Glib::ustring> &, gint32, const Glib::ustring &);
    sigc::signal<void, guint32, const std::vector<Glib::ustring> &, gint32, const Glib::ustring &> Error_signal;
    sigc::signal<void, const std::vector<Glib::ustring>&, guint32, const std::vector<Glib::ustring> &, gint32, const Glib::ustring &> Error_selectiveSignal;

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

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class Thumbnailer1Stub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret(guint32 p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<guint32> var0 =
            Glib::Variant<guint32>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret() {
        std::vector<Glib::VariantBase> vlist;

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret(const std::vector<Glib::ustring> & p0, const std::vector<Glib::ustring> & p1) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<std::vector<Glib::ustring>> var0 =
            Glib::Variant<std::vector<Glib::ustring>>::create(p0);
        vlist.push_back(var0);
        Glib::Variant<std::vector<Glib::ustring>> var1 =
            Glib::Variant<std::vector<Glib::ustring>>::create(p1);
        vlist.push_back(var1);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

    void ret(const std::vector<Glib::ustring> & p0) {
        std::vector<Glib::VariantBase> vlist;
        Glib::Variant<std::vector<Glib::ustring>> var0 =
            Glib::Variant<std::vector<Glib::ustring>>::create(p0);
        vlist.push_back(var0);

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // thumbnails
} // freedesktop
} // org

