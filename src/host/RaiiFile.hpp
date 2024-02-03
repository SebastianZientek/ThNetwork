#pragma once

template <typename FileT>
class RaiiFile
{
public:
    explicit RaiiFile(FileT file)
        : m_file(file)
    {
    }

    RaiiFile(const RaiiFile &) = delete;
    RaiiFile(RaiiFile &&) noexcept = default;
    RaiiFile &operator=(const RaiiFile &) = delete;
    RaiiFile &operator=(RaiiFile &&) noexcept = default;

    ~RaiiFile()  // NOLINT
    {
        m_file.close();
    }

    FileT *operator->()
    {
        return &m_file;
    }

private:
    FileT m_file;
};
