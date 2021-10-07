#pragma once

namespace DragonLib
{
    // Prototype
    class Object;

    class ObjectManager
    {
    public:
        void AppendObject(Object* object);
        void RemoveObject(Object* object);
        void ClearObjects();

    private:
        std::map<std::string, Object*> m_Objects;
    };
}
