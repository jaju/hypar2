/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#pragma once
#include "hypar/localdefs.h"

BEGIN_NAMESPACE (hy);

/**
 * Wrapper call for the mmap (2) call. Easier reading from files!
 */
class Mmap
{
    public:
        typedef enum
        {
            EMMAPINVAL = -100,
            EMMAPFILEREAD,
            OK = 0
        }
        status_t;

    private:
        status_t m_status;
        string m_filename;
        char const *m_pBuffer;
        size_t m_szBuflen;

    private:
        Mmap ();
        inline int init ();

    public:
        /**
         * The constructor, with the file-name
         */
        Mmap (const char *pFileName);
        ~Mmap ();

    public:
        status_t getStatus ();
        /*
         * Get the pointer to the base of the buffer where the file's contents
         * exist
         */
        const char *getBuffer ();
        const char *getFileName ();
        /**
         * Call to get the length of the file
         */
        size_t getBufferLength ();
};

END_NAMESPACE (hy);
