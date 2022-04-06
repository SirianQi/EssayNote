/*
 * @lc app=leetcode.cn id=5 lang=cpp
 *
 * [5] 最长回文子串
 */

// @lc code=start
class Solution {
public:
    string longestPalindrome(string s) {
			int max_len(0), start(0), end(s.size() - 1), min_start(0), max_end(0);

      while(start < end){
        while(end > start){
          if(s[start] == s[end] && isRevertString(s.substr(start, end - start + 1))){
            max_len = max(max_len, end - start + 1); // find max length
            max_end = end;  // update maximal end
            min_start = start; // update minimal start
            end = s.size() - 1; // reset end
          }
          if(end <= max_end){ // no revert
            end = s.size() - 1; // reset end
          }

          end--; // update end
        }
        start++;
        end = s.size() - 1;
      }

      return s.substr(min_start, max_end - min_start + 1);
    }

    bool isRevertString(string s){
      bool result = true;
      for(int start = 0, end = s.size() - 1;start <= s.size()/2 - 1; start++, end--){
        if(s[start] != s[end]){
          result = false;
        }
      }
      return result;
    }
};
// @lc code=end

