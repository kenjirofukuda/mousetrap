//
// Created by clem on 6/23/23.
//

#include <mousetrap/color_chooser.hpp>
#include <mousetrap/log.hpp>

#if GTK_MINOR_VERSION >= 10

namespace mousetrap::detail
{
    DECLARE_NEW_TYPE(ColorChooserInternal,color_chooser_internal, COLOR_CHOOSER_INTERNAL)
    DEFINE_NEW_TYPE_TRIVIAL_INIT(ColorChooserInternal,color_chooser_internal, COLOR_CHOOSER_INTERNAL)

    static void color_chooser_internal_finalize(GObject* object)
    {
        auto* self = MOUSETRAP_COLOR_CHOOSER_INTERNAL(object);
        delete self->on_accept;
        delete self->on_cancel;
        gdk_rgba_free(self->last_color);
        G_OBJECT_CLASS(color_chooser_internal_parent_class)->finalize(object);
    }

    DEFINE_NEW_TYPE_TRIVIAL_CLASS_INIT(ColorChooserInternal,color_chooser_internal, COLOR_CHOOSER_INTERNAL)

    static ColorChooserInternal*color_chooser_internal_new()
    {
        auto* self = (ColorChooserInternal*) g_object_new(color_chooser_internal_get_type(), nullptr);
        color_chooser_internal_init(self);

        self->native = gtk_color_dialog_new();
        gtk_color_dialog_set_with_alpha(self->native, true);

        self->last_color = new GdkRGBA();
        self->last_color->red = 1;
        self->last_color->green = 1;
        self->last_color->blue = 1;
        self->last_color->alpha = 1;

        self->on_accept = nullptr;
        self->on_cancel = nullptr;

        return self;
    }
}

namespace mousetrap
{
    ColorChooser::ColorChooser(const std::string& title)
        : _internal(detail::color_chooser_internal_new())
    {
        g_object_ref(_internal);
        gtk_color_dialog_set_title(_internal->native, title.c_str());
    }

    ColorChooser::ColorChooser(detail::ColorChooserInternal* internal)
        : _internal(g_object_ref(internal))
    {}

    ColorChooser::~ColorChooser()
    {
        g_object_unref(_internal);
    }

    void ColorChooser::present()
    {
        gtk_color_dialog_choose_rgba(
            _internal->native,
            nullptr,
            _internal->last_color,
            (GCancellable*) nullptr,
            on_color_selected_callback,
            _internal
        );
    }

    RGBA ColorChooser::get_color() const
    {
        auto out = RGBA();
        out.r = _internal->last_color->red;
        out.g = _internal->last_color->green;
        out.b = _internal->last_color->blue;
        out.a = _internal->last_color->alpha;

        return out;
    }

    void ColorChooser::on_color_selected_callback(GObject* self, GAsyncResult* result, void* data)
    {
        GError* error_maybe = nullptr;
        auto* result_rgba = gtk_color_dialog_choose_rgba_finish(GTK_COLOR_DIALOG(self), result, &error_maybe);

        using namespace detail;
        auto* instance = MOUSETRAP_COLOR_CHOOSER_INTERNAL(data);
        auto temp = ColorChooser(instance);

        if (error_maybe != nullptr)
        {
            if (error_maybe->code == 2) // dismissed by user
            {
                if (instance->on_cancel != nullptr)
                    (*instance->on_cancel)(temp);
            }
            else
                log::critical("In ColorChooser::on_color_selected: " + std::string(error_maybe->message));

            g_error_free(error_maybe);
            return;
        }

        auto color = RGBA();
        color.r = result_rgba->red;
        color.g = result_rgba->green;
        color.b = result_rgba->blue;
        color.a = result_rgba->alpha;

        if (instance->on_accept != nullptr)
            (*instance->on_accept)(temp, color);

        instance->last_color = gdk_rgba_copy(result_rgba);
        gdk_rgba_free(result_rgba);
    }
}

#endif