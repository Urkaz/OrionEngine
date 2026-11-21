#include "Orion/Core/FileSystem/MemoryMountSource.h"

#include <algorithm>
#include <cstring>
#include <span>

namespace Orion
{
    namespace
    {
        class MemoryFile final : public IFile
        {
        public:
            explicit MemoryFile(std::span<const uint8_t> data) : m_Data(data) {}

            [[nodiscard]] size_t Size() const override
            {
                return m_Data.size();
            }
            [[nodiscard]] size_t Tell() const override
            {
                return m_Position;
            }

            bool Seek(size_t offset) override;
            size_t Read(void* buffer, size_t size) override;
            // read-only
            size_t Write(const void* /*data*/, size_t /*size*/) override
            {
                return 0;
            }

            // nothing is ever buffered
            void Flush() override {}

        private:
            std::span<const uint8_t> m_Data;
            size_t m_Position = 0;
        };
    } // namespace

    bool MemoryFile::Seek(size_t offset)
    {
        if (offset > m_Data.size())
            return false;
        m_Position = offset;
        return true;
    }

    size_t MemoryFile::Read(void* buffer, size_t size)
    {
        const size_t count = std::min(size, m_Data.size() - m_Position);
        if (count > 0)
            std::memcpy(buffer, m_Data.subspan(m_Position, count).data(), count);
        m_Position += count;
        return count;
    }

    void MemoryMountSource::AddFile(std::string relativePath, std::vector<uint8_t> data)
    {
        m_Files[std::move(relativePath)] = std::move(data);
    }

    void MemoryMountSource::AddFile(std::string relativePath, std::string_view text)
    {
        AddFile(std::move(relativePath), std::vector<uint8_t>(text.begin(), text.end()));
    }

    UniquePtr<IFile> MemoryMountSource::Open(std::string_view relativePath, FileMode mode)
    {
        if (mode != FileMode::Read)
            return nullptr;

        const auto it = m_Files.find(relativePath);
        if (it == m_Files.end())
            return nullptr;
        return MakeUnique<MemoryFile>(it->second);
    }

    bool MemoryMountSource::Exists(std::string_view relativePath) const
    {
        return m_Files.contains(relativePath);
    }
} // namespace Orion
