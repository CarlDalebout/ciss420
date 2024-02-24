#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

std::string dec_to_binary(uint32_t x, int bits)
{
    std::string ret = "";
    for(int i = bits-1; i >= 0; --i)
    {
        if(i != 0 && i % 4 == 3)
        {
            ret += ' ';
        }
        ret += (char)(((x) >> i) & 1)+48;
    }
    return ret;
}

struct Block
{
    Block(uint32_t t = 0, uint32_t s = 0, uint32_t sh = 0)
    :tag(t), set(s), shift(sh)
    {}

    inline 
    bool operator==(Block x)
    {
        if (x.tag == tag &&
            x.set == set )
        {
            return true;
        }
        else
            return false;
    }

    inline 
    void print(int tag_bits_, int set_bits_, int shift_bits_)
    {
        std::cout << '|' << dec_to_binary(tag,   tag_bits_)     
                  << '|' << dec_to_binary(set,   set_bits_)     
                  << '|' << dec_to_binary(shift, shift_bits_) << "|\n";
    }

    u_int64_t address;
    u_int32_t tag;
    u_int32_t set;
    u_int32_t shift;
};

class Cache
{
public:
    Cache(int bits_for_memory, int cache_size/*in bytes*/, int block_size/*in bytes*/, int k_associativity = 2, int victim_cache = 2)
    :misses_(0), pa_bits_(bits_for_memory),shift_bits_(std::log2(block_size)), 
     total_lines_(cache_size/block_size), victim_cache_size_(victim_cache)
    {
        set_size_ = total_lines_/k_associativity;
        set_bits_ = std::log2(set_size_);
        tag_bits_ = bits_for_memory - (set_bits_ + shift_bits_);
        cache_ = std::vector<std::vector<Block>>(std::pow(2,set_bits_), std::vector<Block>());
        victim_cache_ = std::vector<Block>();
    }

    inline
    void print_info()
    {
        std::cout << "Tag: " << tag_bits_ << " set: " << set_bits_ << " shift: " << shift_bits_ << "\nBlocks: " << total_lines_ << std::endl;
    }

    void print_cache()
    {
        Block blank(0, 0, 0);
        std::cout << misses_ << std::endl;
        std::cout << std::setfill('-') << std::setw(64) << ' ' << std::endl;
        for(int i = 0; i < (std::pow(2, set_bits_)); ++i)
        {   
            for(int j = 0; j < (total_lines_/set_size_); ++j)
            {   
                if(j < cache_[i].size())
                    cache_[i][j].print(tag_bits_, set_bits_, shift_bits_);
                else
                    blank.print(tag_bits_, set_bits_, shift_bits_);
            }
            std::cout << std::setfill('-') << std::setw(64)  << "^" << i << std::endl;
        }
        std::cout << std::endl << std::setw(64) << std::left << "Victims_cache: " << std::endl;
        for(int i = 0; i < victim_cache_size_; ++i)
        {   
            if(i < victim_cache_.size())
                victim_cache_[i].print(tag_bits_, set_bits_, shift_bits_);
            else
                blank.print(tag_bits_, set_bits_, shift_bits_);
        }
        std::cout << std::setfill('-') << std::setw(64) << ' ' << std::endl;
        std::cout << "to load next address hit (enter), to skip printing n addresses type (skip_), to finish the file type (finish_)\n";
    }

    void push_victum_cache(Block x)
    {
        if(victim_cache_.size() < victim_cache_size_)
        {
            victim_cache_.insert(victim_cache_.begin(), x);
        }
        else
        {
            victim_cache_.erase(victim_cache_.begin());
            victim_cache_.insert(victim_cache_.begin(), x);
        }
    }

    bool check_victum_cache(Block x)
    {
        for(int i = 0; i < victim_cache_.size(); ++i)
        {
            if(x == victim_cache_[i])
            {

                if(cache_[x.set].size() < set_size_)
                {
                    cache_[x.set].push_back(x);
                    victim_cache_.erase(victim_cache_.begin()+i);
                }
                else
                {
                    cache_[x.set].erase(cache_[x.set].begin());
                    cache_[x.set].push_back(x);
                    victim_cache_.erase(victim_cache_.begin()+i);
                }
                return true;
            }
        }
        return false;
    }

    void check_cache(Block x)
    {
        for(int i = 0; i < cache_[x.set].size(); ++i)
        {
            if(x == cache_[x.set][i])
            {
                // std::cout << "hit set:" << x.set << "index: " << i << std::endl;
                cache_[x.set].erase(cache_[x.set].begin()+i);
                cache_[x.set].push_back(x);
                return;
            }
        }

        if(check_victum_cache(x))
        {    
            return;
        }
        else
        {
            misses_++;
            if(cache_[x.set].size() < set_size_)
            {
                cache_[x.set].push_back(x);
            }
            else
            {
                push_victum_cache(cache_[x.set][0]);
                cache_[x.set].erase(cache_[x.set].begin());
                cache_[x.set].push_back(x);
            }
        }
    }

// private:
int misses_;

int pa_bits_;
int tag_bits_;
int set_bits_;
int set_size_;
int shift_bits_;
int total_lines_;
int victim_cache_size_;

std::vector<std::vector<Block>> cache_;
std::vector<Block> victim_cache_;

};

Block get_block(std::string temp, Cache cache)
{
    Block ret;
    temp.erase(0, 2);
    uint32_t temp_int = std::stoi(temp,nullptr,16);
    ret.tag  = temp_int >> (cache.set_bits_ + cache.shift_bits_);
    ret.set  = ((temp_int >> cache.shift_bits_) << (cache.tag_bits_ + cache.shift_bits_)) >> ((cache.tag_bits_ + cache.shift_bits_)) ; // wrong
    ret.shift = (temp_int << (cache.pa_bits_ - cache.shift_bits_)) >> (cache.pa_bits_ - cache.shift_bits_);
    
    return ret;
}

void print_bits(Block temp, Cache cache)
{
    std::cout << std::setw(20) << std::left << ' ' << "   tag: " << dec_to_binary(temp.tag, cache.tag_bits_) << std::endl;
    std::cout << std::setw(20) << std::left << ' ' << "   set:  " << dec_to_binary(temp.set, cache.set_bits_) << std::endl;
    std::cout << std::setw(20) << std::left << ' ' << " shift:  " << dec_to_binary(temp.shift, cache.shift_bits_) << std::endl;
    std::cout << std::setw(64) << std::setfill('-') << " " << std::endl << std::endl;
}

int main()
{

    Cache cache( 32, 512, 64, 2, 2);
    cache.print_info();
    std::cout << cache.victim_cache_.size() << std::endl;
    // cache.print_cache();
    
    std::fstream fsread;
    fsread.open("data.txt");
    
    std::string temp = "";
    bool finish = false;
    int skip = -1;
    int misses = 0;
    while(!fsread.eof())
    {
        bool found = false;
        std::getline(fsread, temp);
        Block temp_block = get_block(temp, cache);
        
        cache.check_cache(temp_block);

        if(!finish) 
        {   
            std::cout << temp.erase(0, 2) << '\t' << dec_to_binary(std::stoi(temp, nullptr, 16), cache.pa_bits_) << std::endl;
            print_bits(temp_block, cache);
            cache.print_cache();
            
            if(skip < 0)
            {
                std::getline(std::cin, temp);
                if(temp == "print_cache")
                {
                }
                else if(temp == "skip_")
                {
                    std::cout << "how many lines do you want to skip: ";
                    int amount = 0;
                    std::cin >> skip;
                }
                else if(temp == "finish_")
                    finish = true;
            }
            skip--;
        }
    }
    std::cout << cache.misses_ << std::endl;
    fsread.close();
    return 0;
}