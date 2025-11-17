#pragma once
#include <iostream>
#include <vector>
#include "glibmm.h"
#include "giomm.h"

namespace org {
namespace freedesktop {
namespace DBus {

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

    static Glib::VariantContainerBase Set_pack(
        const Glib::ustring & arg_interface_name,
        const Glib::ustring & arg_property_name,
        const Glib::VariantBase & arg_value) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<Glib::ustring> interface_name_param =
            Glib::Variant<Glib::ustring>::create(arg_interface_name);
        params.push_back(interface_name_param);

        Glib::Variant<Glib::ustring> property_name_param =
            Glib::Variant<Glib::ustring>::create(arg_property_name);
        params.push_back(property_name_param);

        Glib::Variant<Glib::VariantBase> value_param =
            Glib::Variant<Glib::VariantBase>::create(arg_value);
        params.push_back(value_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

} // DBus
} // freedesktop
} // org

namespace org {
namespace freedesktop {
namespace DBus {

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

} // DBus
} // freedesktop
} // org

namespace org {
namespace freedesktop {
namespace DBus {

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

} // DBus
} // freedesktop
} // org

namespace org {
namespace freedesktop {
namespace thumbnails {

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

} // thumbnails
} // freedesktop
} // org


