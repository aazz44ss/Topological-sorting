# Topological-sorting

拓墣排序後輸出的每個有向邊ＸＹ，Ｘ都會排在Ｙ之前

//先找出那些入邊為0的點

//將入邊為0點X輸出，並且將Ｘ出邊所到達的點Ｙ入邊數-1，並且判斷若是Ｙ入邊變為0則放入最小堆中
