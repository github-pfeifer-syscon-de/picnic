#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include "thumbnail1_common.h"

namespace org {
namespace freedesktop {
namespace DBus {

class PropertiesProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<PropertiesProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<PropertiesProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void Get(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Get_finish (
        Glib::VariantBase &value,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    Glib::VariantBase
    Get_sync(
        const Glib::ustring & interface_name,        const Glib::ustring & property_name,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetAll(
        const Glib::ustring & interface_name,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetAll_finish (
        std::map<Glib::ustring,Glib::VariantBase> &properties,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::map<Glib::ustring,Glib::VariantBase>
    GetAll_sync(
        const Glib::ustring & interface_name,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Set(
        const Glib::ustring & interface_name,
        const Glib::ustring & property_name,
        const Glib::VariantBase & value,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Set_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    Set_sync(
        const Glib::ustring & interface_name,        const Glib::ustring & property_name,        const Glib::VariantBase & value,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    sigc::signal<void, Glib::ustring, std::map<Glib::ustring,Glib::VariantBase>, std::vector<Glib::ustring> > PropertiesChanged_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    PropertiesProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace DBus {

class IntrospectableProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<IntrospectableProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<IntrospectableProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void Introspect(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Introspect_finish (
        Glib::ustring &xml_data,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    Glib::ustring
    Introspect_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);


    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    IntrospectableProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace DBus {

class PeerProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<PeerProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<PeerProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void Ping(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Ping_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    Ping_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetMachineId(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetMachineId_finish (
        Glib::ustring &machine_uuid,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    Glib::ustring
    GetMachineId_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);


    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    PeerProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// DBus
}// freedesktop
}// org
namespace org {
namespace freedesktop {
namespace thumbnails {

class Thumbnailer1Proxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<Thumbnailer1Proxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<Thumbnailer1Proxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void Queue(
        const std::vector<Glib::ustring> & uris,
        const std::vector<Glib::ustring> & mime_types,
        const Glib::ustring & flavor,
        const Glib::ustring & scheduler,
        guint32 handle_to_unqueue,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Queue_finish (
        guint32 &handle,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    guint32
    Queue_sync(
        const std::vector<Glib::ustring> & uris,        const std::vector<Glib::ustring> & mime_types,        const Glib::ustring & flavor,        const Glib::ustring & scheduler,        guint32 handle_to_unqueue,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Dequeue(
        guint32 handle,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Dequeue_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    Dequeue_sync(
        guint32 handle,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetSupported(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetSupported_finish (
        std::vector<Glib::ustring> &uri_schemes,
        std::vector<Glib::ustring> &mime_types,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::tuple<std::vector<Glib::ustring>, std::vector<Glib::ustring>>
    GetSupported_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetSchedulers(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetSchedulers_finish (
        std::vector<Glib::ustring> &schedulers,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::vector<Glib::ustring>
    GetSchedulers_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetFlavors(
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void GetFlavors_finish (
        std::vector<Glib::ustring> &flavors,
        const Glib::RefPtr<Gio::AsyncResult> &res);

    std::vector<Glib::ustring>
    GetFlavors_sync(
const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    sigc::signal<void, guint32 > Started_signal;
    sigc::signal<void, guint32 > Finished_signal;
    sigc::signal<void, guint32, std::vector<Glib::ustring> > Ready_signal;
    sigc::signal<void, guint32, std::vector<Glib::ustring>, gint32, Glib::ustring > Error_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

private:
    Thumbnailer1Proxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

};

}// thumbnails
}// freedesktop
}// org

