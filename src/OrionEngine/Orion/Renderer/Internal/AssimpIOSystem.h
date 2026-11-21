#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

namespace Orion::Internal
{
    // An Orion IFile seen as an Assimp IOStream
    class AssimpIOStream final : public Assimp::IOStream
    {
    public:
        explicit AssimpIOStream(UniquePtr<IFile> file) : m_File(std::move(file)) {}

        size_t Read(void* buffer, size_t size, size_t count) override;
        size_t Write(const void* /*buffer*/, size_t /*size*/, size_t /*count*/) override
        {
            // import only reads
            return 0;
        }

        aiReturn Seek(size_t offset, aiOrigin origin) override;
        [[nodiscard]] size_t Tell() const override
        {
            return m_File->Tell();
        }
        [[nodiscard]] size_t FileSize() const override
        {
            return m_File->Size();
        }
        void Flush() override {}

    private:
        UniquePtr<IFile> m_File;
    };

    // Makes Assimp open every file through FileSystem (model, textures, etc)
    // Importer::SetIOHandler takes OWNERSHIP of this so there is no need to delete it.
    class AssimpIOSystem final : public Assimp::IOSystem
    {
    public:
        using Assimp::IOSystem::Open; // keep the std::string overload visible

        [[nodiscard]] bool Exists(const char* path) const override;
        [[nodiscard]] char getOsSeparator() const override
        {
            return '/';
        }
        Assimp::IOStream* Open(const char* path, const char* mode = "rb") override;
        void Close(Assimp::IOStream* stream) override;
    };
} // namespace Orion::Internal
