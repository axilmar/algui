#ifndef ALGUI_MESSAGES_H
#define ALGUI_MESSAGES_H


/**
 * Messages.
 */
enum ALGUI_MSG {
    /**
     * Allocate memory for an object.
     * Data: [in, out] pointer to allocated memory.
     */
    ALGUI_MSG_MALLOC,

    /**
     * Free object memory.
     * Data: NULL.
     */
     ALGUI_MSG_FREE,

     /**
      * Initialize object.
      * Data: NULL.
      */
      ALGUI_MSG_INIT,

      /**
       * Cleanup object.
       * Data: NULL.
       */
       ALGUI_MSG_CLEANUP
};


#endif //ALGUI_MESSAGES_H
