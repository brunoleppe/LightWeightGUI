#pragma once
#include <SDL2/SDL.h>

namespace lw {

    class NativeWindow;

    /**
     * @brief Interface for objects that can be attached to a NativeWindow.
     * This allows GUI objects (like a Frame) to be bound to a window.
     */
    class IGUIAttachment {
    public:
        virtual ~IGUIAttachment() = default;

        /**
         * @brief Called when the attachment is bound to a window.
         * @param window The window this object is being bound to.
         */
        virtual void onAttach(NativeWindow* window) = 0;

        /**
         * @brief Called when the attachment is unbound from a window.
         */
        virtual void onDetach() = 0;
    
        /**
         * @brief Process an SDL event.
         */
        virtual void onEvent(const SDL_Event& event) = 0;

        /**
         * @brief Render the GUI content.
         */
        virtual void onRender() = 0;
    };

} // namespace lw
