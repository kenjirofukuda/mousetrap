//
// Created by clem on 4/12/23.
//

#include <mousetrap/window.hpp>
#include <mousetrap/header_bar.hpp>
#include <mousetrap/scrolled_window.hpp>
#include <mousetrap/revealer.hpp>
#include <mousetrap/box.hpp>
#include <mousetrap/button.hpp>
#include <mousetrap/application.hpp>

#include "signals_chapter.hpp"
#include "motion_controller_test.hpp"
#include "paned_test.hpp"
#include "sound_test.hpp"
#include "widget_layout_test.hpp"
#include "image_scale_test.hpp"
#include "list_test.hpp"
#include "label_test.hpp"
#include "render_area_test.hpp"
#include "shape_showcase.hpp"

using namespace mousetrap;

static void test()
{
    std::cout << "test called" << std::endl;
}


/// @brief main layout
inline struct State
{
    Window main_window;
    HeaderBar main_window_header_bar;

    Stack stack;
    StackSidebar stack_control = StackSidebar(stack);
    ScrolledWindow stack_control_window;
    Revealer stack_control_revealer;
    Box stack_control_revealer_wrapper = Box(Orientation::VERTICAL);
    Button stack_control_revealer_button = Button();

    Button stack_previous_button = Button("&#9664;"); // left arrow
    Button stack_next_button = Button("&#9654;"); // right arrow

    Box stack_box = Box(Orientation::HORIZONTAL);

    std::vector<Widget*> tests;

}* state = nullptr;

/// @brief add test to state
/// @param test pointer to TestComponent
void add_test(Widget* test, const std::string& title)
{
    assert(state != nullptr);
    state->tests.push_back(test);
    auto _ = state->stack.add_child(hbox(*test), title);
}

/// @brief main
int main()
{
    auto app = Application("docs.screenshots");
    app.connect_signal_activate([](Application* app)
    {
        state = new State{Window(*app)};
        /*
        // setup children

        //add_test(new RenderAreaTest(), "RenderArea");
        add_test(new ShapeShowcase(), "GL Shapes");
        add_test(new MotionControllerTest(), "MotionEventController");
        add_test(new PanedTest(), "Paned");
        add_test(new SoundTest("/home/clem/Workspace/mousetrap/test/test.wav"), "Sound");
        add_test(new WidgetLayoutTest(), "Widget Layout");
        add_test(new ImageScaleTest(), "Image Scaling");
        add_test(new ListTest(), "ListView");

        // action to hide gui element other than stack child

        static auto hide_show_stack_control_action = Action("main.show_hide_stack_control", app);
        hide_show_stack_control_action.set_function([](Action*)
        {
            auto current = state->stack_control_revealer.get_revealed();
            state->stack_control_revealer.set_revealed(not current);

            for (auto* button : {
                &state->stack_previous_button,
                &state->stack_next_button
            })
                button->set_visible(not current);
        });
        hide_show_stack_control_action.add_shortcut("<Control>h");
        state->stack_control_revealer_button.set_action(hide_show_stack_control_action);

        // action to go to next child

        static auto next_child_action = Action("main.stack_next", app);
        next_child_action.set_function([](Action*){

            auto* model = state->stack.get_selection_model();
            size_t current = model->get_selection().at(0);
            size_t max = state->stack.get_n_children();

            if (current+1 < max)
                model->select(current+1);
        });
        next_child_action.add_shortcut("<Control>Right");
        state->stack_next_button.set_action(next_child_action);

        // action to go to previous child

        static auto previous_child_action = Action("main.stack_previous", app);
        previous_child_action.set_function([](Action*){
            auto* model = state->stack.get_selection_model();
            size_t current = model->get_selection().at(0);
            size_t max = state->stack.get_n_children();

            if (current > 0)
                model->select(current-1);
        });
        previous_child_action.add_shortcut("<Control>Left");
        state->stack_previous_button.set_action(previous_child_action);

        // add shortcut binding for all actions

        static auto shortcut_controller = ShortcutController();
        shortcut_controller.add_action(hide_show_stack_control_action);
        state->main_window.add_controller(shortcut_controller);

        for (auto* action : {
        &hide_show_stack_control_action,
        &previous_child_action,
        &next_child_action
        })
            shortcut_controller.add_action(*action);

        // update action availability and window title based on stack selection

        state->stack.get_selection_model()->connect_signal_selection_changed([&](SelectionModel* model, size_t, size_t){
            auto i = model->get_selection().at(0);
            previous_child_action.set_enabled(i != 0);
            next_child_action.set_enabled(i < int(state->stack.get_n_children()) - 1);

            auto* test = state->tests.at(i);
        });
        state->stack.get_selection_model()->emit_signal_selection_changed(0, 0); // update initial state of actions

        // layout

        state->stack.set_expand(true);
        state->stack_control_window.set_expand_horizontally(false);
        state->stack_control_window.set_expand_vertically(true);
        state->stack_control_window.set_propagate_natural_width(true);
        state->stack_control_window.set_propagate_natural_height(true);
        state->stack_control_revealer.set_transition_type(RevealerTransitionType::SLIDE_RIGHT);

        state->stack_box.push_back(state->stack);
        state->stack_control_window.set_child(state->stack_control);
        state->stack_control_revealer.set_child(state->stack_control_window);
        state->stack_control_revealer_wrapper.push_back(state->stack_control_revealer);
            // we need to put the revealer inside its own box to allow the layout manager to reallocate the outer box,
            // properly hiding the revealer by expanding the outer box, as opposed to the revealer simply becoming invsible
            // with empty space left where it has space allocated

        state->stack_box.push_back(state->stack_control_revealer_wrapper);
        state->stack_control_revealer_button.set_has_frame(false);

        state->main_window_header_bar.push_back(state->stack_control_revealer_button);
        state->main_window_header_bar.push_front(state->stack_previous_button);
        state->main_window_header_bar.push_front(state->stack_next_button);

        state->main_window.set_title("mousetrap");
        state->main_window.set_child(state->stack_box);
        state->main_window.set_titlebar_widget(state->main_window_header_bar);

        state->main_window.set_child(state->stack_box);

        auto& window = state->main_window;

        // TODO
        // TODO

        state->main_window.present();
         */

        auto render_area = RenderArea();
        render_area.set_size_request({100, 100});

        g_object_add_toggle_ref(G_OBJECT(render_area.operator GObject*()), (GToggleNotify) test, nullptr);

        state->main_window.set_child(render_area);
        state->main_window.present();
    });

    app.connect_signal_shutdown([](Application*){
        for (auto* test : state->tests)
            delete test;

        delete state;
    });

    return app.run();
}
