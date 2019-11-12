#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

#include <iostream>

using namespace ns3;

class MyObject : public Object
{
public:
  static TypeId GetTypeId (void)
 {
  static TypeId tid = TypeId ("MyObject")
     .SetParent (Object::GetTypeId ())
     .AddConstructor<MyObject> ()
     .AddTraceSource ("MyInteger",
                      "An integer value to trace.",
                      MakeTraceSourceAccessor (&MyObject::m_myInt));

  return tid; 
 }
 
 MyObject () {}
 TracedValue<int32_t> m_myInt;
};

void
IntTrace (int32_t oldValue , int32_t newValue)
{
 std::cout << "Traced " << oldValue  << " to " << newValue << std::endl;
}

void
IntTrace2 (int32_t bu)
{
 std::cout << "Oi, " << bu << "!" << std::endl;
}

int
main (int argc, char *argv[])
{
Ptr<MyObject> myObject = CreateObject<MyObject> ();
myObject->TraceConnectWithoutContext ("MyInteger",MakeCallback(&IntTrace));

//Ptr<MyObject> balu = CreateObject<MyObject> ();
//balu -> TraceConnectWithoutContext ("MyInteger",MakeCallback(&IntTrace2));

myObject->m_myInt = 1234;
myObject->m_myInt = 5678;
//balu->m_myInt = 5678;

}
