#pragma once
#include <iostream>
#include <map>
#include <tuple>
#include "glibmm.h"
#include "giomm.h"

class PropertiesTypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static Glib::VariantContainerBase Get_pack(
        const Glib::ustring & arg_interface_name,
        const Glib::ustring & arg_property_name) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<Glib::ustring> interface_name_param =
            Glib::Variant<Glib::ustring>::create(arg_interface_name);
        params.push_back(interface_name_param);

        Glib::Variant<Glib::ustring> property_name_param =
            Glib::Variant<Glib::ustring>::create(arg_property_name);
        params.push_back(property_name_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase GetAll_pack(
        const Glib::ustring & arg_interface_name) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_interface_name);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

class PropertiesMessageHelper {
public:
    PropertiesMessageHelper (const Glib::RefPtr<Gio::DBus::MethodInvocation> msg):
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

class IntrospectableTypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }
};

class IntrospectableMessageHelper {
public:
    IntrospectableMessageHelper (const Glib::RefPtr<Gio::DBus::MethodInvocation> msg):
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

class PeerTypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }
};

class PeerMessageHelper {
public:
    PeerMessageHelper (const Glib::RefPtr<Gio::DBus::MethodInvocation> msg):
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

class Thumbnailer1TypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static Glib::VariantContainerBase Queue_pack(
        const std::vector<Glib::ustring> & arg_uris,
        const std::vector<Glib::ustring> & arg_mime_types,
        const Glib::ustring & arg_flavor,
        const Glib::ustring & arg_scheduler,
        guint32 arg_handle_to_unqueue) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<std::vector<Glib::ustring>> uris_param =
            Glib::Variant<std::vector<Glib::ustring>>::create(arg_uris);
        params.push_back(uris_param);

        Glib::Variant<std::vector<Glib::ustring>> mime_types_param =
            Glib::Variant<std::vector<Glib::ustring>>::create(arg_mime_types);
        params.push_back(mime_types_param);

        Glib::Variant<Glib::ustring> flavor_param =
            Glib::Variant<Glib::ustring>::create(arg_flavor);
        params.push_back(flavor_param);

        Glib::Variant<Glib::ustring> scheduler_param =
            Glib::Variant<Glib::ustring>::create(arg_scheduler);
        params.push_back(scheduler_param);

        Glib::Variant<guint32> handle_to_unqueue_param =
            Glib::Variant<guint32>::create(arg_handle_to_unqueue);
        params.push_back(handle_to_unqueue_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase Dequeue_pack(
        guint32 arg_handle) {
        Glib::VariantContainerBase base;
        Glib::Variant<guint32> params =
            Glib::Variant<guint32>::create(arg_handle);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

class Thumbnailer1MessageHelper {
public:
    Thumbnailer1MessageHelper (const Glib::RefPtr<Gio::DBus::MethodInvocation> msg):
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

