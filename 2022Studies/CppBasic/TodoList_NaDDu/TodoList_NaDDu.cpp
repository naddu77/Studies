#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <list>
#include <sstream>

struct Strings
{
    char const* exit;
    char const* list_todo_items;
    char const* add_todo_item;
    char const* remove_todo_item;
    char const* manage_todo_item;
    char const* save_todo_list;
    char const* load_todo_list;
    char const* selection;
    char const* error_message;
    char const* add_message;
    char const* remove_message;
    char const* empty_message;
    char const* go_back;
    char const* management_message;
    char const* modification_message;
    char const* status_modification;
    char const* content_modification;
    char const* status_modification_message;
    char const* status1;
    char const* status2;
    char const* status3;
    char const* current_status_message;
    char const* content_modification_message;
};

Strings kor_strings{
    .exit{ "종료" },
    .list_todo_items{ "ToDo List 확인" },
    .add_todo_item{ "ToDo 추가" },
    .remove_todo_item{ "ToDo 삭제" },
    .manage_todo_item{ "ToDo 관리" },
    .save_todo_list{ "저장" },
    .load_todo_list{ "불러오기" },
    .selection{ "작업선택" },
    .error_message{ "잘못 입력하셨습니다." },
    .add_message{ "ToDo내용을 입력 해 주세요" },
    .remove_message{ "삭제할 ToDo의 번호를 입력 해 주세요" },
    .empty_message{ "Todo List가 비어있습니다." },
    .go_back{ "뒤로가기" },
    .management_message{ "내용을 변경할 ToDo를 선택해주세요" },
    .modification_message{ "편집하고 싶은 것의 번호를 선택 해 주세요" },
    .status_modification{ "상태 변경" },
    .content_modification{ "내용 변경" },
    .status_modification_message{ "ToDo 상태를 편집합니다" },
    .status1{ "상태 없음" },
    .status2{ "진행중" },
    .status3{ "완료" },
    .current_status_message{ "현재 상태를 입력 해 주세요" },
    .content_modification_message{ "ToDo 메시지를 편집합니다." }
};

class LocalizedString
{
public:
    enum class Language {
        Korea,
        English,
    };

    ~LocalizedString() = default;
    LocalizedString(LocalizedString const&) = delete;
    LocalizedString(LocalizedString&&) = delete;
    LocalizedString& operator=(LocalizedString const&) = delete;
    LocalizedString& operator=(LocalizedString&&) = delete;

    static LocalizedString& GetInstance(Language language = Language::Korea)
    {
        static LocalizedString instance(language);

        return instance;
    }

    Strings const& GetStrings() const
    {
        return strs;
    }

    void SetLanguage(Language language)
    {
        switch (language)
        {
        case LocalizedString::Language::Korea:
            strs = kor_strings;
            break;
        case LocalizedString::Language::English:
            // Not Supported
            break;
        default:
            break;
        }
    }
private:
    Strings strs;

    LocalizedString(Language language)
    {
        SetLanguage(language);
    }
};

enum class TaskStaus
{
    None,
    Prograss,
    Done,
    End
};

struct ToDoItem
{
    TaskStaus status;
    std::string content;
};

template <typename Elem, typename Traits = std::char_traits<Elem>>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, ToDoItem const& todo_item)
{
    os << "|";

    switch (todo_item.status)
    {
    case TaskStaus::None:
        os << "None";
        break;

    case TaskStaus::Prograss:
        os << "Prograss";
        break;

    case TaskStaus::Done:
        os << "Done";
        break;

    case TaskStaus::End:
        os << "End";
        break;

    default:
        break;
    }

    os << "|" << todo_item.content << "|";

    return os;
}

template <typename Elem, typename Traits = std::char_traits<Elem>>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, std::list<ToDoItem> const& todo_list)
{
    for (auto i{ 0 }; auto const& e : todo_list)
    {
        os << std::format("{} : {}\n", ++i, e);
        //os << ++i << " : " << e << "\n";
    }

    return os;
}

namespace std
{
    template<class CharT>
    struct formatter<TaskStaus, CharT>
    {
        template <typename FormatParseContext>
        auto parse(FormatParseContext& pc)
        {
            // parse formatter args like padding, precision if you support it
            return pc.end(); // returns the iterator to the last parsed character in the format string, in this case we just swallow everything
        }

        template<typename FormatContext>
        auto format(TaskStaus status, FormatContext& fc)
        {
            switch (status)
            {
            case TaskStaus::None:
                return std::format_to(fc.out(), "None");

            case TaskStaus::Prograss:
                return std::format_to(fc.out(), "Prograss");

            case TaskStaus::Done:
                return std::format_to(fc.out(), "Done");

            case TaskStaus::End:
                return std::format_to(fc.out(), "End");

            default:
                return std::format_to(fc.out(), "???");;
            }
        }
    };

    template<class CharT>
    struct formatter<ToDoItem, CharT>
    {
        template <typename FormatParseContext>
        auto parse(FormatParseContext& pc)
        {
            // parse formatter args like padding, precision if you support it
            return pc.end(); // returns the iterator to the last parsed character in the format string, in this case we just swallow everything
        }

        template<typename FormatContext>
        auto format(ToDoItem todo_item, FormatContext& fc)
        {
            return std::format_to(fc.out(), "|{}|{}|", todo_item.status, todo_item.content);
        }
    };
}

void PrintMenu()
{
    auto const& lsm{ LocalizedString::GetInstance().GetStrings() };

    std::cout << "-= Menu =-\n";
    std::cout << std::format("0. {}\n", lsm.exit);
    std::cout << std::format("1. {}\n", lsm.list_todo_items);
    std::cout << std::format("2. {}\n", lsm.add_todo_item);
    std::cout << std::format("3. {}\n", lsm.remove_todo_item);
    std::cout << std::format("4. {}\n", lsm.manage_todo_item);
    std::cout << std::format("5. {}\n", lsm.save_todo_list);
    std::cout << std::format("6. {}\n", lsm.load_todo_list);
    std::cout << std::format("{} : ", lsm.selection);
}

int main()
{
    using enum TaskStaus;

    auto const& lsm{ LocalizedString::GetInstance().GetStrings() };
    std::list<ToDoItem> todo_list;

    PrintMenu();

    for (std::string line; std::getline(std::cin, line) ; PrintMenu())
    {
        try
        {
            auto const i{ std::stol(line) };

            switch (i)
            {
            case 0:
                return 0;
                
            case 1:
                if (std::empty(todo_list))
                {
                    std::cout << lsm.empty_message << std::endl;
                }
                else
                {
                    std::cout << todo_list;
                }
                break;

            case 2:
                std::cout << std::format("{} : ", lsm.add_message);
                
                if (std::getline(std::cin, line))
                {
                    todo_list.emplace_back(None, line);
                }
                else
                {
                    std::cerr << lsm.error_message << std::endl;
                }
                break;

            case 3:
                if (std::empty(todo_list))
                {
                    std::cout << lsm.empty_message << std::endl;
                    
                    break;
                }
                else
                {
                    std::cout << std::format("0. {}\n", lsm.go_back);
                    std::cout << todo_list;
                }

                std::cout << std::format("{} : ", lsm.remove_message);

                if (std::getline(std::cin, line))
                {
                    try
                    {
                        auto const i{ std::stol(line) };

                        if (i == 0)
                        {
                            continue;
                        }

                        if (0 < i and i <= std::ssize(todo_list))
                        {
                            todo_list.erase(std::next(std::begin(todo_list), i - 1));
                        }
                        else
                        {
                            std::cerr << lsm.error_message << std::endl;
                        }
                    }
                    catch (std::exception const& e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                }
                else
                {
                    std::cerr << lsm.error_message << std::endl;
                }
                break;

            case 4:
            {
                auto end_flag{ false };

                while (!end_flag)
                {
                    if (std::empty(todo_list))
                    {
                        std::cout << std::format("0. {}\n", lsm.go_back);
                        std::cout << lsm.empty_message << std::endl;

                        break;
                    }
                    else
                    {
                        std::cout << todo_list;
                        std::cout << std::format("{} : ", lsm.modification_message);
                    }

                    if (std::getline(std::cin, line))
                    {
                        try
                        {
                            auto i{ std::stoi(line) };

                            if (i == 0)
                            {
                                end_flag = true;

                                break;
                            }

                            if (0 < i and i <= std::ssize(todo_list))
                            {
                                auto end_flag2{ false };

                                while (!end_flag2)
                                {
                                    std::cout << std::format("0. {}\n", lsm.go_back);
                                    std::cout << std::format("1. {}\n", lsm.status_modification);
                                    std::cout << std::format("2. {}\n", lsm.content_modification);
                                    std::cout << std::format("{} : ", lsm.modification_message);

                                    if (std::getline(std::cin, line))
                                    {
                                        try
                                        {
                                            switch (std::stoi(line))
                                            {
                                            case 0:
                                                end_flag2 = true;
                                                break;

                                            case 1:
                                            {
                                                auto end_flag3{ false };

                                                while (!end_flag3)
                                                {
                                                    std::cout << lsm.status_modification_message << std::endl;
                                                    std::cout << std::format("0. {}\n", lsm.go_back);
                                                    std::cout << std::format("1. {}\n", lsm.status1);
                                                    std::cout << std::format("2. {}\n", lsm.status2);
                                                    std::cout << std::format("3. {}\n", lsm.status3);
                                                    std::cout << std::format("{} : ", lsm.current_status_message);

                                                    if (std::getline(std::cin, line))
                                                    {
                                                        try
                                                        {
                                                            switch (std::stoi(line))
                                                            {
                                                            case 0:
                                                                end_flag3 = true;
                                                                break;

                                                            case 1:
                                                                [[fallthrough]];
                                                            case 2:
                                                                [[fallthrough]];
                                                            case 3:
                                                                std::next(std::begin(todo_list), i - 1)->status = static_cast<TaskStaus>(i);
                                                                end_flag = true;
                                                                end_flag2 = true;
                                                                end_flag3 = true;
                                                                break;
                                                            }
                                                        }
                                                        catch (std::exception const& e)
                                                        {
                                                            std::cerr << e.what() << std::endl;
                                                        }
                                                    }
                                                }

                                                break;
                                            }

                                            case 2:
                                                std::cout << lsm.content_modification_message << std::endl;
                                                std::cout << std::format("{} : ", lsm.current_status_message);

                                                if (std::getline(std::cin, line))
                                                {
                                                    std::next(std::begin(todo_list), i - 1)->content = line;
                                                    end_flag = true;
                                                    end_flag2 = true;
       
                                                }

                                                break;

                                            default:
                                                std::cerr << lsm.error_message << std::endl;
                                                break;
                                            }
                                        }
                                        catch (std::exception const& e)
                                        {
                                            std::cerr << e.what() << std::endl;
                                        }
                                    }
                                    else
                                    {
                                        std::cerr << lsm.error_message << std::endl;
                                    }
                                }
                            }
                            else
                            {
                                std::cerr << lsm.error_message << std::endl;
                            }
                        }
                        catch (std::exception const& e)
                        {
                            std::cerr << e.what() << std::endl;
                        }  
                    }
                }
                
                break;
            }   
            case 5:
                break;

            case 6:
                break;

            default:
                std::cout << lsm.error_message << std::endl;
                break;
            }
        }
        catch (std::exception const& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}