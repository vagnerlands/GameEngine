#ifndef _KEYS_H_
#define _KEYS_H_

#include "IKeyEvent.h"
#include <functional>

#define BindKey(KeyBinder, Listener) _Keys::KeyBinderDispatcher<KeyBinder>::Instance().Bind(Listener);

class CGameController;

namespace _Keys
{
    // easier to read
    #define KeyClass(val) class Key##val## : public IKey<AscKey_##val##>\
    {\
        public:\
            virtual void OnKeyEvent(const Key##val##& e) {};\
    };

    static const Byte AscKey_Escape = 27;
	static const Byte AscKey_0 = 48;
	static const Byte AscKey_1 = 49;
	static const Byte AscKey_2 = 50;
	static const Byte AscKey_3 = 51;
	static const Byte AscKey_4 = 52;
	static const Byte AscKey_5 = 53;
	static const Byte AscKey_6 = 54;
	static const Byte AscKey_7 = 55;
	static const Byte AscKey_8 = 56;
	static const Byte AscKey_9 = 57;
	static const Byte AscKey_a = 97;
	static const Byte AscKey_b = 98;
	static const Byte AscKey_c = 99;
	static const Byte AscKey_d = 100;
	static const Byte AscKey_e = 101;
	static const Byte AscKey_f = 102;
	static const Byte AscKey_g = 103;
	static const Byte AscKey_h = 104;
	static const Byte AscKey_i = 105;
	static const Byte AscKey_j = 106;
	static const Byte AscKey_k = 107;
	static const Byte AscKey_l = 108;
	static const Byte AscKey_m = 109;
	static const Byte AscKey_n = 110;
	static const Byte AscKey_o = 111;
	static const Byte AscKey_p = 112;
	static const Byte AscKey_q = 113;
	static const Byte AscKey_r = 114;
	static const Byte AscKey_s = 115;
	static const Byte AscKey_t = 116;
	static const Byte AscKey_u = 117;
	static const Byte AscKey_v = 118;
	static const Byte AscKey_w = 119;
	static const Byte AscKey_x = 120;
	static const Byte AscKey_y = 121;
    static const Byte AscKey_z = 122;

    KeyClass(0);
    KeyClass(1);
    KeyClass(2);
    KeyClass(3);
    KeyClass(4);
    KeyClass(5);
    KeyClass(6);
    KeyClass(7);
    KeyClass(8);
    KeyClass(9);
    KeyClass(q);
    KeyClass(w);
    KeyClass(e);
    KeyClass(r);
    KeyClass(t);
    KeyClass(y);
    KeyClass(u);
    KeyClass(i);
    KeyClass(o);
    KeyClass(p);
    KeyClass(a);
    KeyClass(s);
    KeyClass(d);
    KeyClass(f);
    KeyClass(g);
    KeyClass(h);
    KeyClass(j);
    KeyClass(k);
    KeyClass(l);
    KeyClass(z);
    KeyClass(x);
    KeyClass(c);
    KeyClass(v);
    KeyClass(b);
    KeyClass(n);
    KeyClass(m);
    KeyClass(Escape);

	template <typename KeyBinder>
	class KeyBinderDispatcher
	{
        list<KeyBinder*> m_listeners;
        KeyBinderDispatcher() {}
	public:
		static KeyBinderDispatcher& Instance()
		{
			static KeyBinderDispatcher instance;
			return instance;
		}
		~KeyBinderDispatcher() {}
		void Bind(KeyBinder* b) { m_listeners.push_back(b); }
		void Event()
		{
			for (list<KeyBinder*>::iterator i = m_listeners.begin(); i != m_listeners.end(); i++)
			{
				(*i)->OnKeyEvent(KeyBinder());
			}
		}
	};

    // this one doesn't rely on a KeyBinder, but on a single key
    class KeyDispatcher
    {
        typedef std::pair<UByte, std::function<void(Float, UInt32)>> KeyDispatcherEntry;
        list<KeyDispatcherEntry> m_listeners;
    protected:
        KeyDispatcher() {}
        CGameController* mpGameController;
        static KeyDispatcher* pInstance;
    public:        
        ~KeyDispatcher() {}
        static KeyDispatcher& Instance();
        void Bind(UByte c, std::function<void(Float, UInt32)> func) { m_listeners.push_back(std::make_pair(c, func)); }
        void Event(Float dt, UByte key);
    };

    class KeyDispatcherFactory : public KeyDispatcher
    {
        KeyDispatcherFactory(CGameController* pGameController) 
        {
            KeyDispatcher::mpGameController = pGameController;
        }
    public:
        static void Create(CGameController* pGameController);
    };
}
#endif //_KEYS_H_
