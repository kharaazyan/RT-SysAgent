#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "json.hpp"

using json = nlohmann::json;

inline std::string current_timestamp()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm tm_utc;
    gmtime_r(&t, &tm_utc);

    std::stringstream ss;
    ss << std::put_time(&tm_utc, "%Y-%m-%dT%H:%M:%S");
    ss << '.' << std::setw(3) << std::setfill('0') << ms.count() << "Z";
    return ss.str();
}

inline std::string read_prev_cid(const std::string &filepath = "/tmp/prev_cid.txt")
{
    std::ifstream in(filepath);
    if (!in.is_open())
        return "";
    std::string cid;
    std::getline(in, cid);
    return cid;
}

inline void save_prev_cid(const std::string &filepath, const std::string &cid)
{
    std::ofstream out(filepath, std::ios::trunc);
    if (out.is_open())
    {
        out << cid << '\n';
    }
}

inline std::string format_logs_json(const std::vector<std::string> &logs, const std::string &prev_cid)
{
    json j;
    j["timestamp"] = current_timestamp();
    j["logs"] = logs;
    j["prev_cid"] = prev_cid.empty() ? nullptr : prev_cid;
    return j.dump(2);
}

inline std::string run_command(const std::string &cmd)
{
    std::string result;
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return "";
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

inline std::string ipfs_add(const std::string &filepath)
{
    std::string check = run_command("pgrep -x ipfs");
    if (check.empty())
    {
        std::cerr << "[IPFS] Warning: IPFS daemon not running!\n";
    }

    std::string cmd = "ipfs add -q " + filepath;
    std::string output = run_command(cmd);
    if (!output.empty())
    {
        output.erase(output.find_last_not_of(" \n\r\t") + 1);
    }
    else
    {
        std::cerr << "[IPFS] Error: Could not add file.\n";
    }

    return output;
}
