/*
 * Copyright 2004 Ravindra Jaju (jaju [AT] msync org)
 */

/*
 * This file is part of the HyParSuite and usage is governed by the License
 * included with the source-code
 */

#include "mmap.hpp"
#include "debug.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>

BEGIN_NAMESPACE (hy);

Mmap::Mmap (const char *pFileName) :
    m_status (Mmap::EMMAPINVAL),
    m_filename (""),
    m_pBuffer (0),
    m_szBuflen (0)
{
    assert (pFileName);
    m_filename = pFileName;
    m_status = init();
}

inline Mmap::status_t Mmap::init ()
{
    int fd;
    struct stat fstatinfo;

    if ((fd = open (m_filename.c_str (), O_RDONLY)) < 0)
    {
        hy_warn (("Error in read-only open of file %s\n", m_filename.c_str ()));
        return Mmap::EMMAPFILEREAD;
    }

    if (fstat (fd, &fstatinfo) < 0)
    {
        hy_warn (("Error in stat'ting file %s\n", m_filename.c_str ()));
        return Mmap::EMMAPINVAL;
    }

    if (!S_ISREG (fstatinfo.st_mode))
    {
        hy_warn (("Not a regular file %s\n", m_filename.c_str ()));
        return Mmap::EMMAPINVAL;
    }

    m_szBuflen = (size_t) fstatinfo.st_size;

    m_pBuffer = (const char *) mmap (0, m_szBuflen, PROT_READ, MAP_PRIVATE, fd, 0);

    if (m_pBuffer)
    {
        return Mmap::OK;
    }
    else
    {
        return Mmap::EMMAPINVAL;
    }
}

Mmap::~Mmap ()
{
    if (m_pBuffer)
    {
        munmap ((void *) m_pBuffer, m_szBuflen);
    }
    m_pBuffer = 0;
}

Mmap::status_t Mmap::getStatus ()
{
    return m_status;
}

const char *Mmap::getBuffer ()
{
    return m_pBuffer;
}

char *Mmap::getBufferCopy ()
{
    char *copy = new char[m_szBuflen + 1];
    bzero(copy, m_szBuflen + 1);
    memcpy(copy, m_pBuffer, m_szBuflen);
    return copy;
}

const char *Mmap::getFileName ()
{
    return m_filename.c_str ();
}

size_t Mmap::getBufferLength ()
{
    return m_szBuflen;
}

END_NAMESPACE (hy);
