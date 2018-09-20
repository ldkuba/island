#ifndef GUARD_TEST_APP_H
#define GUARD_TEST_APP_H
#endif

#include <stdint.h>
#include "pal_api_loader/ApiRegistry.hpp"

// depends on le_backend_vk. le_backend_vk must be loaded before this class is used.

#ifdef __cplusplus
extern "C" {
#endif

void register_test_app_api( void *api );

struct test_app_o;

// clang-format off
struct test_app_api {

	static constexpr auto id      = "test_app";
	static constexpr auto pRegFun = register_test_app_api;

	struct test_app_interface_t {
		test_app_o * ( *create                   )();
		void         ( *destroy                  )( test_app_o *self );
		bool         ( *update                   )( test_app_o *self );

		void         ( *key_callback             )( void *user_data, int key, int scancode, int action, int mods );
		void         ( *character_callback       )( void *user_data, unsigned int codepoint );
		void         ( *cursor_position_callback )( void *user_data, double xpos, double ypos );
		void         ( *cursor_enter_callback    )( void *user_data, int entered );
		void         ( *mouse_button_callback    )( void *user_data, int button, int action, int mods );
		void         ( *scroll_callback          )( void *user_data, double xoffset, double yoffset );

		void         ( *initialize               )(); // static methods
		void         ( *terminate                )(); // static methods
	};

	test_app_interface_t test_app_i;
};
// clang-format on

#ifdef __cplusplus
} // extern "C"

namespace test_app {
#ifdef PLUGINS_DYNAMIC
const auto api = Registry::addApiDynamic<test_app_api>( true );
#else
const auto api = Registry::addApiStatic<test_app_api>();
#endif

static const auto &test_app_i = api -> test_app_i;

} // namespace test_app

class TestApp : NoCopy, NoMove {

	test_app_o *self;

  public:
	TestApp()
	    : self( test_app::test_app_i.create() ) {
	}

	bool update() {
		return test_app::test_app_i.update( self );
	}

	~TestApp() {
		test_app::test_app_i.destroy( self );
	}

	static void initialize() {
		test_app::test_app_i.initialize();
	}

	static void terminate() {
		test_app::test_app_i.terminate();
	}
};

#endif