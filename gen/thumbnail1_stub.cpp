static const char interfaceXml0[] = R"XML_DELIMITER(<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"
                      "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">
<!-- GDBus 2.86.1 -->
<node>
  <interface name="org.freedesktop.DBus.Properties">
    <method name="Get">
      <arg type="s" name="interface_name" direction="in"/>
      <arg type="s" name="property_name" direction="in"/>
      <arg type="v" name="value" direction="out"/>
    </method>
    <method name="GetAll">
      <arg type="s" name="interface_name" direction="in"/>
      <arg type="a{sv}" name="properties" direction="out"/>
    </method>
    <method name="Set">
      <arg type="s" name="interface_name" direction="in"/>
      <arg type="s" name="property_name" direction="in"/>
      <arg type="v" name="value" direction="in"/>
    </method>
    <signal name="PropertiesChanged">
      <arg type="s" name="interface_name"/>
      <arg type="a{sv}" name="changed_properties"/>
      <arg type="as" name="invalidated_properties"/>
    </signal>
  </interface>
  <interface name="org.freedesktop.DBus.Introspectable">
    <method name="Introspect">
      <arg type="s" name="xml_data" direction="out"/>
    </method>
  </interface>
  <interface name="org.freedesktop.DBus.Peer">
    <method name="Ping"/>
    <method name="GetMachineId">
      <arg type="s" name="machine_uuid" direction="out"/>
    </method>
  </interface>
  <interface name="org.freedesktop.thumbnails.Thumbnailer1">
    <method name="Queue">
      <arg type="as" name="uris" direction="in"/>
      <arg type="as" name="mime_types" direction="in"/>
      <arg type="s" name="flavor" direction="in"/>
      <arg type="s" name="scheduler" direction="in"/>
      <arg type="u" name="handle_to_unqueue" direction="in"/>
      <arg type="u" name="handle" direction="out"/>
    </method>
    <method name="Dequeue">
      <arg type="u" name="handle" direction="in"/>
    </method>
    <method name="GetSupported">
      <arg type="as" name="uri_schemes" direction="out"/>
      <arg type="as" name="mime_types" direction="out"/>
    </method>
    <method name="GetSchedulers">
      <arg type="as" name="schedulers" direction="out"/>
    </method>
    <method name="GetFlavors">
      <arg type="as" name="flavors" direction="out"/>
    </method>
    <signal name="Started">
      <arg type="u" name="handle"/>
    </signal>
    <signal name="Finished">
      <arg type="u" name="handle"/>
    </signal>
    <signal name="Ready">
      <arg type="u" name="handle"/>
      <arg type="as" name="uris"/>
    </signal>
    <signal name="Error">
      <arg type="u" name="handle"/>
      <arg type="as" name="failed_uris"/>
      <arg type="i" name="error_code"/>
      <arg type="s" name="message"/>
    </signal>
  </interface>
</node>
)XML_DELIMITER";

#include "thumbnail1_stub.h"

template<class T>
inline T specialGetter(Glib::Variant<T> variant)
{
    return variant.get();
}

template<>
inline std::string specialGetter(Glib::Variant<std::string> variant)
{
    // String is not guaranteed to be null-terminated, so don't use ::get()
    gsize n_elem;
    gsize elem_size = sizeof(char);
    char* data = (char*)g_variant_get_fixed_array(variant.gobj(), &n_elem, elem_size);

    return std::string(data, n_elem);
}

org::freedesktop::DBus::PropertiesStub::PropertiesStub():
    m_interfaceName("org.freedesktop.DBus.Properties")
{
    PropertiesChanged_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &PropertiesStub::PropertiesChanged_emitter),
            std::vector<Glib::ustring>({""})) );
    PropertiesChanged_selectiveSignal.connect(sigc::mem_fun(this, &PropertiesStub::PropertiesChanged_emitter));
}

org::freedesktop::DBus::PropertiesStub::~PropertiesStub()
{
    unregister_object();
}

guint org::freedesktop::DBus::PropertiesStub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what().c_str());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &PropertiesStub::on_method_call),
            sigc::mem_fun(this, &PropertiesStub::on_interface_get_property),
            sigc::mem_fun(this, &PropertiesStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Properties"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what().c_str());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void org::freedesktop::DBus::PropertiesStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::freedesktop::DBus::PropertiesStub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("Get") == 0) {
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = specialGetter(base_interface_name);

        Glib::Variant<Glib::ustring> base_property_name;
        parameters.get_child(base_property_name, 1);
        Glib::ustring p_property_name = specialGetter(base_property_name);

        MethodInvocation methodInvocation(invocation);
        Get(
            (p_interface_name),
            (p_property_name),
            methodInvocation);
    }

    if (method_name.compare("GetAll") == 0) {
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = specialGetter(base_interface_name);

        MethodInvocation methodInvocation(invocation);
        GetAll(
            (p_interface_name),
            methodInvocation);
    }

    if (method_name.compare("Set") == 0) {
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = specialGetter(base_interface_name);

        Glib::Variant<Glib::ustring> base_property_name;
        parameters.get_child(base_property_name, 1);
        Glib::ustring p_property_name = specialGetter(base_property_name);

        Glib::Variant<Glib::VariantBase> base_value;
        parameters.get_child(base_value, 2);
        Glib::VariantBase p_value = specialGetter(base_value);

        MethodInvocation methodInvocation(invocation);
        Set(
            (p_interface_name),
            (p_property_name),
            (p_value),
            methodInvocation);
    }

}

void org::freedesktop::DBus::PropertiesStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

}

bool org::freedesktop::DBus::PropertiesStub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}

void org::freedesktop::DBus::PropertiesStub::PropertiesChanged_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & interface_name, const std::map<Glib::ustring,Glib::VariantBase> & changed_properties, const std::vector<Glib::ustring> & invalidated_properties)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((interface_name)));;
    paramsList.push_back(Glib::Variant<std::map<Glib::ustring,Glib::VariantBase>>::create((changed_properties)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((invalidated_properties)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.freedesktop.DBus.Properties",
                    "PropertiesChanged",
                    bus_name,
                    params);
        }
    }
}


bool org::freedesktop::DBus::PropertiesStub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}org::freedesktop::DBus::IntrospectableStub::IntrospectableStub():
    m_interfaceName("org.freedesktop.DBus.Introspectable")
{
}

org::freedesktop::DBus::IntrospectableStub::~IntrospectableStub()
{
    unregister_object();
}

guint org::freedesktop::DBus::IntrospectableStub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what().c_str());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &IntrospectableStub::on_method_call),
            sigc::mem_fun(this, &IntrospectableStub::on_interface_get_property),
            sigc::mem_fun(this, &IntrospectableStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Introspectable"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what().c_str());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void org::freedesktop::DBus::IntrospectableStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::freedesktop::DBus::IntrospectableStub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("Introspect") == 0) {
        MethodInvocation methodInvocation(invocation);
        Introspect(
            methodInvocation);
    }

}

void org::freedesktop::DBus::IntrospectableStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

}

bool org::freedesktop::DBus::IntrospectableStub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}


bool org::freedesktop::DBus::IntrospectableStub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}org::freedesktop::DBus::PeerStub::PeerStub():
    m_interfaceName("org.freedesktop.DBus.Peer")
{
}

org::freedesktop::DBus::PeerStub::~PeerStub()
{
    unregister_object();
}

guint org::freedesktop::DBus::PeerStub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what().c_str());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &PeerStub::on_method_call),
            sigc::mem_fun(this, &PeerStub::on_interface_get_property),
            sigc::mem_fun(this, &PeerStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Peer"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what().c_str());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void org::freedesktop::DBus::PeerStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::freedesktop::DBus::PeerStub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("Ping") == 0) {
        MethodInvocation methodInvocation(invocation);
        Ping(
            methodInvocation);
    }

    if (method_name.compare("GetMachineId") == 0) {
        MethodInvocation methodInvocation(invocation);
        GetMachineId(
            methodInvocation);
    }

}

void org::freedesktop::DBus::PeerStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

}

bool org::freedesktop::DBus::PeerStub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}


bool org::freedesktop::DBus::PeerStub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}org::freedesktop::thumbnails::Thumbnailer1Stub::Thumbnailer1Stub():
    m_interfaceName("org.freedesktop.thumbnails.Thumbnailer1")
{
    Started_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &Thumbnailer1Stub::Started_emitter),
            std::vector<Glib::ustring>({""})) );
    Started_selectiveSignal.connect(sigc::mem_fun(this, &Thumbnailer1Stub::Started_emitter));
    Finished_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &Thumbnailer1Stub::Finished_emitter),
            std::vector<Glib::ustring>({""})) );
    Finished_selectiveSignal.connect(sigc::mem_fun(this, &Thumbnailer1Stub::Finished_emitter));
    Ready_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &Thumbnailer1Stub::Ready_emitter),
            std::vector<Glib::ustring>({""})) );
    Ready_selectiveSignal.connect(sigc::mem_fun(this, &Thumbnailer1Stub::Ready_emitter));
    Error_signal.connect(sigc::bind<0>(sigc::mem_fun(this, &Thumbnailer1Stub::Error_emitter),
            std::vector<Glib::ustring>({""})) );
    Error_selectiveSignal.connect(sigc::mem_fun(this, &Thumbnailer1Stub::Error_emitter));
}

org::freedesktop::thumbnails::Thumbnailer1Stub::~Thumbnailer1Stub()
{
    unregister_object();
}

guint org::freedesktop::thumbnails::Thumbnailer1Stub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what().c_str());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &Thumbnailer1Stub::on_method_call),
            sigc::mem_fun(this, &Thumbnailer1Stub::on_interface_get_property),
            sigc::mem_fun(this, &Thumbnailer1Stub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.thumbnails.Thumbnailer1"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what().c_str());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("Queue") == 0) {
        Glib::Variant<std::vector<Glib::ustring>> base_uris;
        parameters.get_child(base_uris, 0);
        std::vector<Glib::ustring> p_uris = specialGetter(base_uris);

        Glib::Variant<std::vector<Glib::ustring>> base_mime_types;
        parameters.get_child(base_mime_types, 1);
        std::vector<Glib::ustring> p_mime_types = specialGetter(base_mime_types);

        Glib::Variant<Glib::ustring> base_flavor;
        parameters.get_child(base_flavor, 2);
        Glib::ustring p_flavor = specialGetter(base_flavor);

        Glib::Variant<Glib::ustring> base_scheduler;
        parameters.get_child(base_scheduler, 3);
        Glib::ustring p_scheduler = specialGetter(base_scheduler);

        Glib::Variant<guint32> base_handle_to_unqueue;
        parameters.get_child(base_handle_to_unqueue, 4);
        guint32 p_handle_to_unqueue = specialGetter(base_handle_to_unqueue);

        MethodInvocation methodInvocation(invocation);
        Queue(
            (p_uris),
            (p_mime_types),
            (p_flavor),
            (p_scheduler),
            (p_handle_to_unqueue),
            methodInvocation);
    }

    if (method_name.compare("Dequeue") == 0) {
        Glib::Variant<guint32> base_handle;
        parameters.get_child(base_handle, 0);
        guint32 p_handle = specialGetter(base_handle);

        MethodInvocation methodInvocation(invocation);
        Dequeue(
            (p_handle),
            methodInvocation);
    }

    if (method_name.compare("GetSupported") == 0) {
        MethodInvocation methodInvocation(invocation);
        GetSupported(
            methodInvocation);
    }

    if (method_name.compare("GetSchedulers") == 0) {
        MethodInvocation methodInvocation(invocation);
        GetSchedulers(
            methodInvocation);
    }

    if (method_name.compare("GetFlavors") == 0) {
        MethodInvocation methodInvocation(invocation);
        GetFlavors(
            methodInvocation);
    }

}

void org::freedesktop::thumbnails::Thumbnailer1Stub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

}

bool org::freedesktop::thumbnails::Thumbnailer1Stub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::Started_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,guint32 handle)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.freedesktop.thumbnails.Thumbnailer1",
                    "Started",
                    bus_name,
                    params);
        }
    }
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::Finished_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,guint32 handle)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.freedesktop.thumbnails.Thumbnailer1",
                    "Finished",
                    bus_name,
                    params);
        }
    }
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::Ready_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,guint32 handle, const std::vector<Glib::ustring> & uris)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((uris)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.freedesktop.thumbnails.Thumbnailer1",
                    "Ready",
                    bus_name,
                    params);
        }
    }
}

void org::freedesktop::thumbnails::Thumbnailer1Stub::Error_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,guint32 handle, const std::vector<Glib::ustring> & failed_uris, gint32 error_code, const Glib::ustring & message)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((failed_uris)));;
    paramsList.push_back(Glib::Variant<gint32>::create((error_code)));;
    paramsList.push_back(Glib::Variant<Glib::ustring>::create((message)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.freedesktop.thumbnails.Thumbnailer1",
                    "Error",
                    bus_name,
                    params);
        }
    }
}


bool org::freedesktop::thumbnails::Thumbnailer1Stub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}
