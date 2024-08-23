#ifndef _BULK_H_
#define _BULK_H_

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>

class cmd_c
{
    std::string m_val;
public:
    virtual void execute() = 0;
    cmd_c(const char *txt) : m_val(txt)
    {}
    std::string to_string()
    {
        return m_val;
    }
};
using sp_cmd_t = std::shared_ptr<cmd_c>;
using cmd_vector_t = std::vector<sp_cmd_t>;
using i_cmd_vector_t = cmd_vector_t::iterator;

class fake_cmd_c : public cmd_c
{
public:
    virtual void execute()
    {
    }

    fake_cmd_c(const char *txt) : cmd_c(txt)
    {}

    static sp_cmd_t build_cmd(const char  *txt)
    {
        sp_cmd_t sp_cmd( new fake_cmd_c(txt));
        return sp_cmd;
    }

};


class bulker_c
{
    static int N;
    cmd_vector_t m_commands;
    int         m_depth;

    bulker_c() : m_depth(0)
    {
    }

    bulker_c(const char *txt) : m_depth(0)
    {
        sp_cmd_t sp_cmd(new fake_cmd_c(txt));
        m_commands.push_back(sp_cmd);
    }

public:
    // singleton pattern
    static bulker_c& get_instance() 
    {
        static bulker_c instance;
        return instance;
    }

    void add(sp_cmd_t &sp_cmd)
    {
        m_commands.push_back(sp_cmd);
        if ( (0 == m_depth) and 
             (N == m_commands.size()) )
        {
            drop();
        }
    }

    void down()
    {
        if ( 0 == m_depth and m_commands.size() > 0 )
        {
            // drop anything we already have at top level
            drop();
        }
        m_depth++; 
    }

    void up()
    {
        m_depth--;
        if (0 == m_depth) 
        {
            drop();
        }
    }

    static std::string compose_log_name()
    {
        const std::time_t now = std::time(nullptr) ; // get the current time point   
        const std::tm calendar_time = *std::localtime( std::addressof(now) ) ;
        std::ostringstream ostr;
        ostr << "bulk" 
             << std::setfill('0') << std::setw(2) << calendar_time.tm_hour 
             << std::setfill('0') << std::setw(2) << calendar_time.tm_min 
             << std::setfill('0') << std::setw(2) << calendar_time.tm_sec
             << ".log" ;
        return ostr.str();
    }

    void output(const std::string &s, std::ofstream &logf)
    {
        std::cout << s;
        logf << s;
    }

    void drop()
    {
        if (0 != m_depth or m_commands.size() == 0 )
        {
            return;
        }
        std::ofstream logf;
        std::string file_name(compose_log_name());
        logf.open(file_name);
        output("bulk: ", logf);
        // to determine last iteration use hand made cycle
        i_cmd_vector_t it = m_commands.begin();
        while ( it != m_commands.end() )
        {
            (*it)->execute();
            output( (*it)->to_string() , logf);
            it++;
            if (it  != m_commands.end() )
            {
                output(", ", logf);
            }
        }
        output( "\n", logf);
        logf.close();
        m_commands.clear();
    }

    static bool set_n(int n)
    {
        if ( n > 0 and n < 100 )
        {
            N = n;
            return true;
        }
        return false;
    }

    static void usage()
    {
        std::cout << "usage bulk -n 3\n";
    }
};

#endif /* _BULK_H_ */
