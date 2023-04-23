#pragma once
#include <string>
#include <glibmm.h>
#include <giomm.h>
#include "thumbnail1_common.h"

namespace org {
namespace freedesktop {
namespace DBus {

class Properties : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<Properties> createForBusFinish (Glib::RefPtr<Gio::AsyncResult> result);

    void Get(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void Get_finish (
        Glib::VariantBase &value,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void GetAll(
        const Glib::ustring & interface_name,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void GetAll_finish (
        std::map<Glib::ustring,Glib::VariantBase> &properties,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    template <typename T>
    void Set(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        T value,
        const Gio::SlotAsyncReady &callback,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {})
    {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;
        Glib::Variant<Glib::ustring> interface_name_param =
    Glib::Variant<Glib::ustring>::create(interface_name);
        params.push_back(interface_name_param);
        Glib::Variant<Glib::ustring> property_name_param =
    Glib::Variant<Glib::ustring>::create(property_name);
        params.push_back(property_name_param);
        Glib::Variant<Glib::Variant<T>> value_variantValue =
            Glib::Variant<Glib::Variant<T>>::create(Glib::Variant<T>::create(value));
        params.push_back(value_variantValue);
        base = Glib::VariantContainerBase::create_tuple(params);

        m_proxy->call( "Set", callback, cancellable, base);
    }

    void Set_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    sigc::signal<void, Glib::ustring, std::map<Glib::ustring,Glib::VariantBase>, std::vector<Glib::ustring> > PropertiesChanged_signal;

    void reference() {}
    void unreference() {}

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

private:
    Properties(Glib::RefPtr<Gio::DBus::Proxy> proxy): Glib::ObjectBase() {
        this->m_proxy = proxy;
        this->m_proxy->signal_signal().connect(sigc::mem_fun(this, &Properties::handle_signal));
    }
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace DBus {

class Introspectable : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<Introspectable> createForBusFinish (Glib::RefPtr<Gio::AsyncResult> result);

    void Introspect(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void Introspect_finish (
        Glib::ustring &xml_data,
        const Glib::RefPtr<Gio::AsyncResult> &res);


    void reference() {}
    void unreference() {}

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

private:
    Introspectable(Glib::RefPtr<Gio::DBus::Proxy> proxy): Glib::ObjectBase() {
        this->m_proxy = proxy;
        this->m_proxy->signal_signal().connect(sigc::mem_fun(this, &Introspectable::handle_signal));
    }
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace DBus {

class Peer : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<Peer> createForBusFinish (Glib::RefPtr<Gio::AsyncResult> result);

    void Ping(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void Ping_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void GetMachineId(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void GetMachineId_finish (
        Glib::ustring &machine_uuid,
        const Glib::RefPtr<Gio::AsyncResult> &res);


    void reference() {}
    void unreference() {}

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

private:
    Peer(Glib::RefPtr<Gio::DBus::Proxy> proxy): Glib::ObjectBase() {
        this->m_proxy = proxy;
        this->m_proxy->signal_signal().connect(sigc::mem_fun(this, &Peer::handle_signal));
    }
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace thumbnails {

class Thumbnailer1 : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<Thumbnailer1> createForBusFinish (Glib::RefPtr<Gio::AsyncResult> result);

    void Queue(
        const std::vector<Glib::ustring> & uris,
        const std::vector<Glib::ustring> & mime_types,
        const Glib::ustring & flavor,
        const Glib::ustring & scheduler,
        guint32 handle_to_unqueue,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void Queue_finish (
        guint32 &handle,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void Dequeue(
        guint32 handle,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void Dequeue_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void GetSupported(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void GetSupported_finish (
        std::vector<Glib::ustring> &uri_schemes,
        std::vector<Glib::ustring> &mime_types,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void GetSchedulers(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void GetSchedulers_finish (
        std::vector<Glib::ustring> &schedulers,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void GetFlavors(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    void GetFlavors_finish (
        std::vector<Glib::ustring> &flavors,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    sigc::signal<void, guint32 > Started_signal;
    sigc::signal<void, guint32 > Finished_signal;
    sigc::signal<void, guint32, std::vector<Glib::ustring> > Ready_signal;
    sigc::signal<void, guint32, std::vector<Glib::ustring>, gint32, Glib::ustring > Error_signal;

    void reference() {}
    void unreference() {}

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

private:
    Thumbnailer1(Glib::RefPtr<Gio::DBus::Proxy> proxy): Glib::ObjectBase() {
        this->m_proxy = proxy;
        this->m_proxy->signal_signal().connect(sigc::mem_fun(this, &Thumbnailer1::handle_signal));
    }
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;
};

}// thumbnails
}// freedesktop
}// org

