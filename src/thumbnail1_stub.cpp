static const char interfaceXml0[] = R"XML_DELIMITER(<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"
                      "http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">
<!-- GDBus 2.58.1 -->
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

org::freedesktop::DBus::Properties::Properties():
    connectionId(0),
    registeredId(0),
    m_interfaceName("org.freedesktop.DBus.Properties")
{
    PropertiesChanged_signal.connect(sigc::mem_fun(this, &Properties::PropertiesChanged_emitter));
}

org::freedesktop::DBus::Properties::~Properties()
{
}

guint org::freedesktop::DBus::Properties::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!m_objectPath.empty() && m_objectPath != object_path) {
        g_warning("Cannot register the same object twice!");
        return 0;
    }

    try {
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
    } catch(const Glib::Error& ex) {
        g_warning("Unable to create introspection data: ");
        g_warning("%s\n", ex.what().c_str());
    }
    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &Properties::on_method_call),
            sigc::mem_fun(this, &Properties::on_interface_get_property),
            sigc::mem_fun(this, &Properties::on_interface_set_property));
    guint id = 0;
    try {
        id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Properties"),
            *interface_vtable);
        m_connection = connection;
        m_objectPath = object_path;
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object failed");
    }
    return id;
}

void org::freedesktop::DBus::Properties::connect(
    Gio::DBus::BusType busType,
    std::string name)
{
    connectionId = Gio::DBus::own_name(
        busType, name,
        sigc::mem_fun(this, &Properties::on_bus_acquired),
        sigc::mem_fun(this, &Properties::on_name_acquired),
        sigc::mem_fun(this, &Properties::on_name_lost));
}

void org::freedesktop::DBus::Properties::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    if (method_name.compare("Get") == 0) {
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = base_interface_name.get();

        Glib::Variant<Glib::ustring> base_property_name;
        parameters.get_child(base_property_name, 1);
        Glib::ustring p_property_name = base_property_name.get();

        Get(
            (p_interface_name),
            (p_property_name),
            PropertiesMessageHelper(invocation));
    }

    if (method_name.compare("GetAll") == 0) {
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = base_interface_name.get();

        GetAll(
            (p_interface_name),
            PropertiesMessageHelper(invocation));
    }

    if (method_name.compare("Set") == 0) {
        Glib::VariantContainerBase containerBase = parameters;
        Glib::Variant<Glib::ustring> base_interface_name;
        parameters.get_child(base_interface_name, 0);
        Glib::ustring p_interface_name = base_interface_name.get();

        Glib::Variant<Glib::ustring> base_property_name;
        parameters.get_child(base_property_name, 1);
        Glib::ustring p_property_name = base_property_name.get();

        GVariant *output2;
        g_variant_get_child(containerBase.gobj(), 2, "v", &output2);
        Glib::VariantBase p_value =
            Glib::VariantBase(output2);

        Set(
            (p_interface_name),
            (p_property_name),
            (p_value),
            PropertiesMessageHelper(invocation));
    }

}

void org::freedesktop::DBus::Properties::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name)
{
}

bool org::freedesktop::DBus::Properties::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    return true;
}

void org::freedesktop::DBus::Properties::PropertiesChanged_emitter(Glib::ustring interface_name, std::map<Glib::ustring,Glib::VariantBase> changed_properties, std::vector<Glib::ustring> invalidated_properties)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((interface_name)));;
    paramsList.push_back(Glib::Variant<std::map<Glib::ustring,Glib::VariantBase>>::create((changed_properties)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((invalidated_properties)));;

    m_connection->emit_signal(
        "/org/freedesktop/DBus/Properties",
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        Glib::ustring(),
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList));
}

void org::freedesktop::DBus::Properties::on_bus_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
    registeredId = register_object(connection,
                                   "/org/freedesktop/DBus/Properties");
    m_connection = connection;
}

void org::freedesktop::DBus::Properties::on_name_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* name */)
{
}

void org::freedesktop::DBus::Properties::on_name_lost(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
}

bool org::freedesktop::DBus::Properties::emitSignal(
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

    m_connection->emit_signal(
        m_objectPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        Glib::ustring(),
        propertiesChangedVariant);

    return true;
}org::freedesktop::DBus::Introspectable::Introspectable():
    connectionId(0),
    registeredId(0),
    m_interfaceName("org.freedesktop.DBus.Introspectable")
{
}

org::freedesktop::DBus::Introspectable::~Introspectable()
{
}

guint org::freedesktop::DBus::Introspectable::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!m_objectPath.empty() && m_objectPath != object_path) {
        g_warning("Cannot register the same object twice!");
        return 0;
    }

    try {
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
    } catch(const Glib::Error& ex) {
        g_warning("Unable to create introspection data: ");
        g_warning("%s\n", ex.what().c_str());
    }
    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &Introspectable::on_method_call),
            sigc::mem_fun(this, &Introspectable::on_interface_get_property),
            sigc::mem_fun(this, &Introspectable::on_interface_set_property));
    guint id = 0;
    try {
        id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Introspectable"),
            *interface_vtable);
        m_connection = connection;
        m_objectPath = object_path;
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object failed");
    }
    return id;
}

void org::freedesktop::DBus::Introspectable::connect(
    Gio::DBus::BusType busType,
    std::string name)
{
    connectionId = Gio::DBus::own_name(
        busType, name,
        sigc::mem_fun(this, &Introspectable::on_bus_acquired),
        sigc::mem_fun(this, &Introspectable::on_name_acquired),
        sigc::mem_fun(this, &Introspectable::on_name_lost));
}

void org::freedesktop::DBus::Introspectable::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    if (method_name.compare("Introspect") == 0) {
        Introspect(
            IntrospectableMessageHelper(invocation));
    }

}

void org::freedesktop::DBus::Introspectable::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name)
{
}

bool org::freedesktop::DBus::Introspectable::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    return true;
}

void org::freedesktop::DBus::Introspectable::on_bus_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
    registeredId = register_object(connection,
                                   "/org/freedesktop/DBus/Introspectable");
    m_connection = connection;
}

void org::freedesktop::DBus::Introspectable::on_name_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* name */)
{
}

void org::freedesktop::DBus::Introspectable::on_name_lost(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
}

bool org::freedesktop::DBus::Introspectable::emitSignal(
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

    m_connection->emit_signal(
        m_objectPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        Glib::ustring(),
        propertiesChangedVariant);

    return true;
}org::freedesktop::DBus::Peer::Peer():
    connectionId(0),
    registeredId(0),
    m_interfaceName("org.freedesktop.DBus.Peer")
{
}

org::freedesktop::DBus::Peer::~Peer()
{
}

guint org::freedesktop::DBus::Peer::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!m_objectPath.empty() && m_objectPath != object_path) {
        g_warning("Cannot register the same object twice!");
        return 0;
    }

    try {
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
    } catch(const Glib::Error& ex) {
        g_warning("Unable to create introspection data: ");
        g_warning("%s\n", ex.what().c_str());
    }
    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &Peer::on_method_call),
            sigc::mem_fun(this, &Peer::on_interface_get_property),
            sigc::mem_fun(this, &Peer::on_interface_set_property));
    guint id = 0;
    try {
        id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.DBus.Peer"),
            *interface_vtable);
        m_connection = connection;
        m_objectPath = object_path;
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object failed");
    }
    return id;
}

void org::freedesktop::DBus::Peer::connect(
    Gio::DBus::BusType busType,
    std::string name)
{
    connectionId = Gio::DBus::own_name(
        busType, name,
        sigc::mem_fun(this, &Peer::on_bus_acquired),
        sigc::mem_fun(this, &Peer::on_name_acquired),
        sigc::mem_fun(this, &Peer::on_name_lost));
}

void org::freedesktop::DBus::Peer::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    if (method_name.compare("Ping") == 0) {
        Ping(
            PeerMessageHelper(invocation));
    }

    if (method_name.compare("GetMachineId") == 0) {
        GetMachineId(
            PeerMessageHelper(invocation));
    }

}

void org::freedesktop::DBus::Peer::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name)
{
}

bool org::freedesktop::DBus::Peer::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    return true;
}

void org::freedesktop::DBus::Peer::on_bus_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
    registeredId = register_object(connection,
                                   "/org/freedesktop/DBus/Peer");
    m_connection = connection;
}

void org::freedesktop::DBus::Peer::on_name_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* name */)
{
}

void org::freedesktop::DBus::Peer::on_name_lost(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
}

bool org::freedesktop::DBus::Peer::emitSignal(
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

    m_connection->emit_signal(
        m_objectPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        Glib::ustring(),
        propertiesChangedVariant);

    return true;
}org::freedesktop::thumbnails::Thumbnailer1::Thumbnailer1():
    connectionId(0),
    registeredId(0),
    m_interfaceName("org.freedesktop.thumbnails.Thumbnailer1")
{
    Started_signal.connect(sigc::mem_fun(this, &Thumbnailer1::Started_emitter));
    Finished_signal.connect(sigc::mem_fun(this, &Thumbnailer1::Finished_emitter));
    Ready_signal.connect(sigc::mem_fun(this, &Thumbnailer1::Ready_emitter));
    Error_signal.connect(sigc::mem_fun(this, &Thumbnailer1::Error_emitter));
}

org::freedesktop::thumbnails::Thumbnailer1::~Thumbnailer1()
{
}

guint org::freedesktop::thumbnails::Thumbnailer1::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!m_objectPath.empty() && m_objectPath != object_path) {
        g_warning("Cannot register the same object twice!");
        return 0;
    }

    try {
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
    } catch(const Glib::Error& ex) {
        g_warning("Unable to create introspection data: ");
        g_warning("%s\n", ex.what().c_str());
    }
    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(this, &Thumbnailer1::on_method_call),
            sigc::mem_fun(this, &Thumbnailer1::on_interface_get_property),
            sigc::mem_fun(this, &Thumbnailer1::on_interface_set_property));
    guint id = 0;
    try {
        id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.freedesktop.thumbnails.Thumbnailer1"),
            *interface_vtable);
        m_connection = connection;
        m_objectPath = object_path;
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object failed");
    }
    return id;
}

void org::freedesktop::thumbnails::Thumbnailer1::connect(
    Gio::DBus::BusType busType,
    std::string name)
{
    connectionId = Gio::DBus::own_name(
        busType, name,
        sigc::mem_fun(this, &Thumbnailer1::on_bus_acquired),
        sigc::mem_fun(this, &Thumbnailer1::on_name_acquired),
        sigc::mem_fun(this, &Thumbnailer1::on_name_lost));
}

void org::freedesktop::thumbnails::Thumbnailer1::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    if (method_name.compare("Queue") == 0) {
        Glib::Variant<std::vector<Glib::ustring>> base_uris;
        parameters.get_child(base_uris, 0);
        std::vector<Glib::ustring> p_uris = base_uris.get();

        Glib::Variant<std::vector<Glib::ustring>> base_mime_types;
        parameters.get_child(base_mime_types, 1);
        std::vector<Glib::ustring> p_mime_types = base_mime_types.get();

        Glib::Variant<Glib::ustring> base_flavor;
        parameters.get_child(base_flavor, 2);
        Glib::ustring p_flavor = base_flavor.get();

        Glib::Variant<Glib::ustring> base_scheduler;
        parameters.get_child(base_scheduler, 3);
        Glib::ustring p_scheduler = base_scheduler.get();

        Glib::Variant<guint32> base_handle_to_unqueue;
        parameters.get_child(base_handle_to_unqueue, 4);
        guint32 p_handle_to_unqueue = base_handle_to_unqueue.get();

        Queue(
            (p_uris),
            (p_mime_types),
            (p_flavor),
            (p_scheduler),
            (p_handle_to_unqueue),
            Thumbnailer1MessageHelper(invocation));
    }

    if (method_name.compare("Dequeue") == 0) {
        Glib::Variant<guint32> base_handle;
        parameters.get_child(base_handle, 0);
        guint32 p_handle = base_handle.get();

        Dequeue(
            (p_handle),
            Thumbnailer1MessageHelper(invocation));
    }

    if (method_name.compare("GetSupported") == 0) {
        GetSupported(
            Thumbnailer1MessageHelper(invocation));
    }

    if (method_name.compare("GetSchedulers") == 0) {
        GetSchedulers(
            Thumbnailer1MessageHelper(invocation));
    }

    if (method_name.compare("GetFlavors") == 0) {
        GetFlavors(
            Thumbnailer1MessageHelper(invocation));
    }

}

void org::freedesktop::thumbnails::Thumbnailer1::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name)
{
}

bool org::freedesktop::thumbnails::Thumbnailer1::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &sender,
    const Glib::ustring &object_path,
    const Glib::ustring &interface_name,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    return true;
}

void org::freedesktop::thumbnails::Thumbnailer1::Started_emitter(guint32 handle)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;

    m_connection->emit_signal(
        "/org/freedesktop/thumbnails/Thumbnailer1",
        "org.freedesktop.thumbnails.Thumbnailer1",
        "Started",
        Glib::ustring(),
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList));
}

void org::freedesktop::thumbnails::Thumbnailer1::Finished_emitter(guint32 handle)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;

    m_connection->emit_signal(
        "/org/freedesktop/thumbnails/Thumbnailer1",
        "org.freedesktop.thumbnails.Thumbnailer1",
        "Finished",
        Glib::ustring(),
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList));
}

void org::freedesktop::thumbnails::Thumbnailer1::Ready_emitter(guint32 handle, std::vector<Glib::ustring> uris)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((uris)));;

    m_connection->emit_signal(
        "/org/freedesktop/thumbnails/Thumbnailer1",
        "org.freedesktop.thumbnails.Thumbnailer1",
        "Ready",
        Glib::ustring(),
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList));
}

void org::freedesktop::thumbnails::Thumbnailer1::Error_emitter(guint32 handle, std::vector<Glib::ustring> failed_uris, gint32 error_code, Glib::ustring message)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<guint32>::create((handle)));;
    paramsList.push_back(Glib::Variant<std::vector<Glib::ustring>>::create((failed_uris)));;
    paramsList.push_back(Glib::Variant<gint32>::create((error_code)));;
    paramsList.push_back(Glib::Variant<Glib::ustring>::create((message)));;

    m_connection->emit_signal(
        "/org/freedesktop/thumbnails/Thumbnailer1",
        "org.freedesktop.thumbnails.Thumbnailer1",
        "Error",
        Glib::ustring(),
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList));
}

void org::freedesktop::thumbnails::Thumbnailer1::on_bus_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
    registeredId = register_object(connection,
                                   "/org/freedesktop/thumbnails/Thumbnailer1");
    m_connection = connection;
}

void org::freedesktop::thumbnails::Thumbnailer1::on_name_acquired(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* name */)
{
}

void org::freedesktop::thumbnails::Thumbnailer1::on_name_lost(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &/* name */)
{
}

bool org::freedesktop::thumbnails::Thumbnailer1::emitSignal(
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

    m_connection->emit_signal(
        m_objectPath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        Glib::ustring(),
        propertiesChangedVariant);

    return true;
}
