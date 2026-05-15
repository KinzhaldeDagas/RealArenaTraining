#pragma once
#include <string>
#include <vector>

namespace io
{
    struct CSVRow
    {
        std::vector<std::string> cols;
    };

    class CSVWriter
    {
    public:
        explicit CSVWriter(const std::wstring& path);
        ~CSVWriter();

        CSVWriter(const CSVWriter&) = delete;
        CSVWriter& operator=(const CSVWriter&) = delete;
        CSVWriter(CSVWriter&&) = delete;
        CSVWriter& operator=(CSVWriter&&) = delete;

        bool ok() const noexcept { return _ok; }
        void write_header(const std::vector<std::string>& cols);
        void write_row(const CSVRow& row);

    private:
        void write_line(const std::string& line);
        static std::string escape(const std::string& s);

        void* _fh = nullptr; // FILE*
        bool _ok = false;
    };
}
