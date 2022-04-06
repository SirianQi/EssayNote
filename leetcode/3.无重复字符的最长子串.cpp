/*
 * @lc app=leetcode.cn id=3 lang=cpp
 *
 * [3] 无重复字符的最长子串
 */

// @lc code=start
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if(s.size() < 1){
            return 0;
        }
        if(s.size() == 1){
            return 1;
        }

        unordered_map<char, int> map_char_pos;
        int max_len = 0;
        int length = 0;
        int cur_start = 0;
        int cur_end = 1;
        map_char_pos[s[cur_start]] = cur_start;
        for(;cur_end<s.size();){
            if (map_char_pos.find(s[cur_end]) != map_char_pos.end()&& map_char_pos[s[cur_end]] >= cur_start){
                max_len = max(max_len, length);    // update max_len
                cur_start = map_char_pos[s[cur_end]] + 1;    // update start cursor
            }
            map_char_pos[s[cur_end]] = cur_end; // update repeated char position
            cur_end++;  // update end cursor
            length = cur_end - cur_start;   // update current length
        }
        
        max_len = max(max_len, length);

        return max_len;
    }
};
// @lc code=end

