* Gói phần mềm sử dựng thêm: Thư viện Timer (Timer.h + Timer.cpp) dùng để đo thời gian.

* Chương trình chính gồm các file: 
	+ npuzzle.cpp, đọc dữ liệu từ file npuzzle.txt.
	Cấu trúc file npuzzle.txt: 	Dòng đầu có 1 số nguyên N là kích cỡ của Puzzle. (N*N)
								Sau đó là N dòng, mỗi dòng N số nguyên biểu diễn trạng thái ban đầu của đề bài. (0 tượng trưng cho ô trống).
	+ Algorithm.cpp: Lưu hàm tìm kiếm và 1 số hàm liên quan.
	+ Heuristic.cpp: Lưu các hàm Heuristic.
	
* Các chương trình dùng trong quá trình đánh giá hiệu năng:
	+ analyzing_tool.cpp: Chạy 1000 trường hợp ngẫu nhiên của 8-Puzzle, lưu lại đề bài trong file startState.txt; lời giải cho các đề bài đó trong file result.txt, thống kê trong file analysis-8puzzle.txt
	+ theorem2.cpp: Dùng để chứng minh Theorem2. Sinh toàn bộ 362880 trường hợp của 8-Puzzle, sau đó tìm lời giải cho từng trường hợp, lưu đề bài trong file theorem2.txt, lời giải trong file theorem2_result.txt
	+ analyzing_pythagorean_heuristic.cpp: Thử độ tin cậy của hàm Heuristic Pythagorean. Sinh 1000 trường hợp ngẫu nhiên của 16-Puzzle, dùng thuật toán IDA* và hàm Heuristic Pythagorean để giải. Đề bài lưu trong file pythagorean_start_state.txt, lời giải lưu trong file pythagorean_result.txt, thống kê trong file pythagorean_analysis.txt.
	