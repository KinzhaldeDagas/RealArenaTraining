#include "CSVWriter.h"
#include <cstdio>
#include <cstring>

using io::CSVWriter;

CSVWriter::CSVWriter(const std::wstring& path)
{
    _fh = _wfopen(path.c_str(), L"wb");
    _ok = (_fh != nullptr);
    if (_ok)
    {
        auto* f = (FILE*)_fh;

        // Disable stdio buffering so the export is immediately visible on disk.
        // (Without this, the file can appear blank until the CS exits because
        // the FILE* is never flushed/closed.)
        std::setvbuf(f, nullptr, _IONBF, 0);
    }
}

CSVWriter::~CSVWriter()
{
    if (_fh)
    {
        auto* f = (FILE*)_fh;
        std::fflush(f);
        std::fclose(f);
        _fh = nullptr;
    }
    _ok = false;
}

static void fputs_u8(FILE* f, const char* s)
{
    std::fwrite(s, 1, std::strlen(s), f);
}

void CSVWriter::write_line(const std::string& line)
{
    if (!_ok) return;
    auto* f = (FILE*)_fh;
    std::fwrite(line.data(), 1, line.size(), f);
    std::fwrite("\r\n", 1, 2, f);
    std::fflush(f);
}

std::string CSVWriter::escape(const std::string& s)
{
    bool needs = false;
    for (char c : s)
    {
        if (c == '"' || c == ',' || c == '\n' || c == '\r')
        {
            needs = true;
            break;
        }
    }
    if (!needs) return s;

    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('"');
    for (char c : s)
    {
        if (c == '"') out.push_back('"');
        out.push_back(c);
    }
    out.push_back('"');
    return out;
}

void CSVWriter::write_header(const std::vector<std::string>& cols)
{
    CSVRow r;
    r.cols = cols;
    write_row(r);
}

void CSVWriter::write_row(const CSVRow& row)
{
    if (!_ok) return;
    std::string line;
    for (size_t i = 0; i < row.cols.size(); ++i)
    {
        if (i) line.push_back(',');
        line += escape(row.cols[i]);
    }
    write_line(line);
}
