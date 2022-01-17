#include "ItemTree.h"
#include <iostream>
#include <format>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <ranges>
#include <list>
#include <sstream>
#include <functional>
#include <optional>
#include <fstream>

struct Strings
{
    char const* menu;
    char const* exit;
    char const* list_todo_items;
    char const* add_todo_item;
    char const* remove_todo_item;
    char const* manage_todo_item;
    char const* edit_todo_item;
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
    char const* succeeded;
    char const* failed;
    char const* go_main_menu;
    char const* exit_message;
    char const* menu_selection_message;
    char const* save_error_message;
    char const* load_error_message;
};

Strings kor_strings{
    .menu{ "-= ToDo 리스트 메뉴 =-" },
    .exit{ "종료" },
    .list_todo_items{ "ToDo List 확인" },
    .add_todo_item{ "ToDo 추가" },
    .remove_todo_item{ "ToDo 삭제" },
    .manage_todo_item{ "ToDo 관리" },
    .edit_todo_item{ " ToDo 편집 메뉴" },
    .save_todo_list{ "저장" },
    .load_todo_list{ "불러오기" },
    .selection{ "작업선택" },
    .error_message{ "잘못 입력하셨습니다." },
    .add_message{ "ToDo 내용을 입력 해 주세요" },
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
    .content_modification_message{ "ToDo 내용 편집합니다" },
    .succeeded{ "작업을 완료했습니다." },
    .failed{ "작업을 실패했습니다." },
    .go_main_menu{ "메인 메뉴로 돌아가시겠습니까(Y/N)? " },
    .exit_message{ "프로그램을 종료합니다." },
    .menu_selection_message{ "원하는 메뉴의 번호를 입력 해 주세요" },
    .save_error_message{ "저장에 실패하였습니다." },
    .load_error_message{ "열기에 실패하였습니다." }
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

    static Strings const& Get(Language language = Language::Korea)
    {
        static LocalizedString instance(language);

        return instance.strs;
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
    Progress,
    Done,
    End
};

struct ToDoItem
{
    TaskStaus status;
    std::string content;
};

template <typename Elem, typename Traits = std::char_traits<Elem>>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, TaskStaus const& task_status)
{
    switch (task_status)
    {
    case TaskStaus::None:
        os << "None";
        break;

    case TaskStaus::Progress:
        os << "Progress";
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

    return os;
}

template <typename Elem, typename Traits = std::char_traits<Elem>>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, ToDoItem const& todo_item)
{
    os << "|";
    os << todo_item.status;
    os << "|" << todo_item.content << "|";

    return os;
}

template <typename Elem, typename Traits = std::char_traits<Elem>>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os, std::list<ToDoItem> const& todo_list)
{
    for (auto const& e : todo_list)
    {
        os << e << '\n';
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

            case TaskStaus::Progress:
                return std::format_to(fc.out(), "Progress");

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

struct History;
class MenuManager;

enum class Result
{
    Done,
    Failed,
    GoBack,
    Exit
};

using ToDoList = std::list<ToDoItem>;
using Action = std::function<Result(MenuManager&)>;

class MenuItem
{
public:
    MenuItem() = default;

    explicit MenuItem(std::string_view title, Action const& action)
        : title{ title }, action{ action }
    {

    }

    ~MenuItem() = default;

    std::string_view GetTitle() const
    {
        return title;
    }

    Result operator()(MenuManager& menu_manager) const
    {
        return std::invoke(action, menu_manager);
    }

private:
    std::string title;
    Action action;
};

class SubMenuItem
{
public:
    SubMenuItem() = default;

    explicit SubMenuItem(std::string_view title)
        : title{ title }
    {

    }

    ~SubMenuItem() = default;

    std::string_view GetTitle() const
    {
        return title;
    }

private:
    std::string title;
};

struct History
    : std::vector<std::pair<ItemTree<MenuItem, SubMenuItem>*, int>>
{

};

template<class... Ts> 
struct Overloaded 
    : Ts... 
{ 
    using Ts::operator()...;
};

class MenuManager
{
public:
    template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, MenuItem>
    Result operator()(U&& menu_item)
    {
        auto const& lsm{ LocalizedString::Get() };

        if (auto result{ std::invoke(std::forward<U>(menu_item), *this) }; result != Result::Failed)
        {
            if (result == Result::Done)
            {
                std::cout << lsm.succeeded << std::endl;
            }

            return result;
        }
        else
        {
            std::cout << lsm.failed << std::endl;

            while (true)
            {
                std::cout << lsm.go_main_menu;

                if (std::string line; std::getline(std::cin, line))
                {
                    try
                    {
                        if (std::size(line) == 1)
                        {
                            auto const answer{ std::toupper(line.front()) };

                            if (answer == 'Y')
                            {
                                break;
                            }
                            else if (answer == 'N')
                            {
                                return Result::GoBack;
                            }
                            else
                            {
                                std::cout << lsm.error_message << std::endl;
                            }
                        }
                    }
                    catch (...)
                    {
                        std::cout << lsm.error_message << std::endl;
                    }
                }
            }

            return Result::Done;   
        }
    }

    template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, ItemTree<MenuItem, SubMenuItem>>
    Result operator()(U&& item_tree)
    {
        auto end_flag{ false };
        std::optional<int> i;

        while (!end_flag)
        {
            auto const menu_count{ PrintMenu(std::forward<U>(item_tree)) };

            i = SelectMenuItem(menu_count);

            if (i)
            {
                end_flag = true;
            }
            else
            {
                std::cout << LocalizedString::Get().error_message << std::endl;
            }
        }

        if (*i == 0)
        {
            if (std::empty(history))
            {
                return Result::Exit;
            }
            else
            {
                return Result::GoBack;
            }
        }

        if (auto result{ GoNext(item_tree, *i) }; result == Result::GoBack)
        {
            return GoBack();
        }
        else
        {
            history.pop_back();

            return result;
        }
    }

    int PrintToDoList() const
    {
        auto i{ 0 };

        if (auto const& lsm{ LocalizedString::Get() }; std::empty(todo_list))
        {
            std::cout << lsm.empty_message << std::endl;
        }
        else
        {
            for (auto const& todo : todo_list)
            {
                std::cout << std::format("{}. {}\n", ++i, todo);
            }
        }

        return i;
    }

    decltype(auto) AddToDo(TaskStaus task_status, std::string_view content)
    {
        return todo_list.emplace_back(task_status, std::data(content));
    }

    auto GetToDoCount() const
    {
        return std::size(todo_list);
    }

    auto EraseToDo(int i) noexcept
    {
        return todo_list.erase(std::next(std::begin(todo_list), i - 1));
    }

    std::optional<int> SelectMenuItem(int menu_count) const
    {
        for (std::string line; std::getline(std::cin, line);)
        {
            try
            {
                if (auto i{ std::stoi(line) }; 0 <= i && i <= menu_count)
                {
                    return i;
                }
                else
                {
                    break;
                }
            }
            catch (...)
            {
                break;
            }
        }

        return std::nullopt;
    }

    void SetToDoStatus(int todo_item_index, TaskStaus task_status)
    {
        std::next(std::begin(todo_list), todo_item_index - 1)->status = task_status;
    }

    void SetToDoContent(int todo_item_index, std::string_view content)
    {
        std::next(std::begin(todo_list), todo_item_index - 1)->content = content;
    }

    template <typename Func>
    auto LineProcessing(Func&& func)
    {
        auto const& lsm{ LocalizedString::Get() };

        if (std::string line; std::getline(std::cin, line))
        {
            return std::invoke(std::forward<Func>(func), std::move(line));
        }
        else
        {
            std::cerr << lsm.error_message << std::endl;

            return Result::Failed;
        }
    }

    auto begin()
    {
        return std::begin(todo_list);
    }

    auto end()
    {
        return std::end(todo_list);
    }

private:
    ToDoList todo_list;
    History history;

    int PrintMenu(ItemTree<MenuItem, SubMenuItem> const& item_tree) const
    {
        auto const& lsm{ LocalizedString::Get() };

        std::cout << item_tree.item.GetTitle() << '\n';

        auto i{ 0 };

        std::cout << std::format("0. {}\n",
            std::empty(history) ? lsm.exit : lsm.go_back
        );

        for (auto const& item : item_tree)
        {
            std::visit(Overloaded{
            [&i](MenuItem const& menu_item) { std::cout << std::format("{}. {}\n", ++i, menu_item.GetTitle()); },
            [&i](ItemTree<MenuItem, SubMenuItem> const& sub_menu) { std::cout << std::format("{}. {}\n", ++i, sub_menu.item.GetTitle()); }
                }, item);
        }

        std::cout << lsm.menu_selection_message << " : ";

        return i;
    }

    Result GoBack()
    {
        auto [prev_menu, _] { history.back()};

        history.pop_back();

        return std::invoke(std::ref(*this), *prev_menu);

    }

    Result GoNext(ItemTree<MenuItem, SubMenuItem>& item_tree, int i)
    {
        history.emplace_back(&item_tree, i);

        return std::visit(std::ref(*this), item_tree[i - 1]);
    }
};

int main()
{
    auto const& lsm{ LocalizedString::Get() };
    auto menu{ ItemTree<MenuItem, SubMenuItem>(lsm.menu) };
    auto callback{ [](std::string_view msg) {
        return [msg](MenuManager& menu_manager) {
            std::cout << msg << std::endl;

            return Result::Done;
        };
    } };

    menu
        // 1. ToDo List 출력
        .Add(
            lsm.list_todo_items,
            [](MenuManager& menu_manager) {
                std::cout << std::format("> {}\n", LocalizedString::Get().list_todo_items);

                menu_manager.PrintToDoList();

                return Result::Done;
            }
        )
        // 2. ToDo 추가
        .Add(
            lsm.add_todo_item,
            [](MenuManager& menu_manager) {
                auto const& lsm{ LocalizedString::Get() };

                std::cout << std::format("{} : ", lsm.add_message);

                return menu_manager.LineProcessing([&](std::string&& line) {
                    menu_manager.AddToDo(TaskStaus::None, line);

                    return Result::Done;
                });
            }
        )
        // 3. ToDo 제거
        .Add(
            lsm.remove_todo_item,
            [](MenuManager& menu_manager) {
                auto const& lsm{ LocalizedString::Get() };
                auto menu_count{ 0 };

                while (true)
                {
                    if (menu_manager.GetToDoCount() == 0)
                    {
                        std::cout << lsm.empty_message << std::endl;

                        return Result::Done;
                    }
                    else
                    {
                        std::cout << std::format("0. {}\n", lsm.go_back);
                        menu_count = menu_manager.PrintToDoList();
                    }

                    std::cout << std::format("{} : ", lsm.remove_message);

                    if (auto i{ menu_manager.SelectMenuItem(menu_count) })
                    {
                        if (*i == 0)
                        {
                            return Result::GoBack;
                        }

                        menu_manager.EraseToDo(*i);

                        return Result::Done;
                    }

                    std::cerr << lsm.error_message << std::endl;
                }
            }
        )
        // 4. ToDo 관리
        .Add(
            lsm.manage_todo_item,
            [](MenuManager& menu_manager) {
                auto const& lsm{ LocalizedString::Get() };
                auto menu_count{ 0 };

                while (true)
                {
                    if (menu_manager.GetToDoCount() == 0)
                    {
                        std::cout << lsm.empty_message << std::endl;

                        return Result::Done;
                    }
                    else
                    {
                        std::cout << std::format("0. {}\n", lsm.go_back);
                        menu_count = menu_manager.PrintToDoList();
                    }

                    std::cout << std::format("{} : ", lsm.modification_message);

                    if (auto const i{ menu_manager.SelectMenuItem(menu_count) })
                    {
                        if (*i == 0)
                        {
                            return Result::GoBack;
                        }

                        auto const result{ menu_manager(
                            ItemTree<MenuItem, SubMenuItem>(lsm.edit_todo_item)
                            .Add(
                                ItemTree<MenuItem, SubMenuItem>(lsm.status_modification)
                                .Add(
                                    lsm.status1,
                                    [i = *i](MenuManager& menu_manager) {
                                        menu_manager.SetToDoStatus(i, TaskStaus::None);

                                        return Result::Done;
                                    }
                                )
                                .Add(
                                    lsm.status2,
                                    [i = *i](MenuManager& menu_manager) {
                                        menu_manager.SetToDoStatus(i, TaskStaus::Progress);

                                        return Result::Done;
                                    }
                                )
                                .Add(
                                    lsm.status3,
                                    [i = *i](MenuManager& menu_manager) {
                                        menu_manager.SetToDoStatus(i, TaskStaus::Done);

                                        return Result::Done;
                                    }
                                )
                            )
                            .Add(
                                lsm.content_modification,
                                [i = *i](MenuManager& menu_manager) {
                                    auto const& lsm{ LocalizedString::Get() };

                                    std::cout << std::format("{} : ", lsm.content_modification_message);

                                    return menu_manager.LineProcessing([&](std::string&& line) {
                                        menu_manager.SetToDoContent(i, line);

                                        return Result::Done;
                                    });
                                }
                            )
                        ) };

                        switch (result)
                        {
                        case Result::Done:
                            [[fallthrough]];
                        case Result::Failed:
                            [[fallthrough]];
                        case Result::Exit:
                            return result;

                        case Result::GoBack:
                            break;

                        default:
                            break;
                        }
                    }

                    std::cerr << lsm.error_message << std::endl;
                }
            }
        )
        // 5. ToDo List 저장
        .Add(
            lsm.save_todo_list,
            [](MenuManager& menu_manager) {
                auto const& lsm{ LocalizedString::Get() };
                std::ofstream ofs("data.dat");

                if (ofs.is_open())
                {
                    for (auto const& todo : menu_manager)
                    {
                        ofs << todo.status << '\n';
                        ofs << todo.content << '\n';
                    }
                }
                else
                {
                    std::cout << lsm.save_error_message << std::endl;
                }

                return Result::Done;
            }
        )
        // 6. ToDo List 열기
        .Add(
            lsm.load_todo_list,
            [](MenuManager& menu_manager) {
                auto const& lsm{ LocalizedString::Get() };
                std::ifstream ifs("data.dat");

                if (ifs.is_open())
                {
                    ToDoItem todo_item;

                    for (std::string line1, line2; std::getline(ifs, line1) && std::getline(ifs, line2); )
                    {
                        TaskStaus task_status;

                        switch (line1.front())
                        {
                        case 'N':
                            task_status = TaskStaus::None;
                            break;

                        case 'P':
                            task_status = TaskStaus::Progress;
                            break;

                        case 'D':
                            task_status = TaskStaus::Done;
                            break;

                        case 'E':
                            task_status = TaskStaus::End;
                            break;

                        default:
                            task_status = TaskStaus::None;
                            break;
                        }

                        menu_manager.AddToDo(task_status, line2);
                    }
                }
                else
                {
                    std::cout << lsm.load_error_message << std::endl;
                }

                return Result::Done;
            }
        )
        ;

    MenuManager menu_manager;

    while (menu_manager(menu) != Result::Exit);

    std::cout << lsm.exit_message << std::endl;
}
