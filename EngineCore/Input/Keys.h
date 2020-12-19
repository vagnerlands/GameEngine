#ifndef _KEYS_H_
#define _KEYS_H_

#include "IKeyEvent.h"

#define BindKey(KeyBinder, Listener) _Keys::KeyBinderDispatcher<KeyBinder>::Instance().Bind(Listener);

namespace _Keys
{
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

	class Key0 : public IKey<AscKey_0>
	{
	public:
		virtual void OnKeyEvent(const Key0& e) {};
	};

	class Key1 : public IKey<AscKey_1>
	{
	public:
		virtual void OnKeyEvent(const Key1& e) {};
	};

	class Key2 : public IKey<AscKey_2>
	{
	public:
		virtual void OnKeyEvent(const Key2& e) {};
	};

	class Key3 : public IKey<AscKey_3>
	{
	public:
		virtual void OnKeyEvent(const Key3& e) {};
	};

	class KeyT : public IKey<AscKey_t>
	{
	public:
		virtual void OnKeyEvent(const KeyT& e) {};
	};

	class KeyY : public IKey<AscKey_y>
	{
	public:
		virtual void OnKeyEvent(const KeyY& e) {};
	};

	class KeyG : public IKey<AscKey_g>
	{
	public:
		virtual void OnKeyEvent(const KeyG& e) {};
	};

	class KeyH : public IKey<AscKey_h>
	{
	public:
		virtual void OnKeyEvent(const KeyH& e) {};
	};

	template <typename KeyBinder>
	class KeyBinderDispatcher
	{
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
	private:
		list<KeyBinder*> m_listeners;
		KeyBinderDispatcher()
		{

		}
	};
}
#endif //_KEYS_H_
