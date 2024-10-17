#include <memory>
#include <string>
#include <vector>

namespace // An unnamed namespace is used to limit the scope of these classes to this translation unit
{
class Menu
{
public:
    virtual ~Menu() = default;           // Virtual destructor for proper cleanup of derived classes
    virtual void show() const = 0;       // Pure virtual function to display the menu, must be overridden
    virtual void select(int index) const = 0; // Pure virtual function for selecting a menu item by index
};

class Builder
{
public:
    virtual ~Builder() = default; // Virtual destructor to allow safe destruction of derived builders
    virtual Builder& withTitle(std::string title) = 0; // Pure virtual function to set the title
    virtual Builder& withBorder(int pixels) = 0; // Pure virtual function to set the border size
    virtual Builder& addOption(std::string option) = 0; // Pure virtual function to add an option to the menu
    virtual Builder& horizontal() = 0; // Pure virtual function to set the layout to horizontal
    virtual Builder& vertical() = 0;   // Pure virtual function to set the layout to vertical
    virtual std::unique_ptr<Menu> build() = 0; // Pure virtual function to build and return the Menu object
};

class ProductionMenu : public Menu
{
public:
    class ProductionBuilder : public Builder
    {
    public:
        ProductionBuilder(std::string id)
            : mMenu{new ProductionMenu{id}} // Constructor initializes mMenu with a new ProductionMenu object
        {
        }

        Builder& withTitle(std::string title) override // Override to set the title of the menu
        {
            mMenu->mTitle = title; // Sets the menu title
            return *this;           // Return reference to the builder for method chaining
        }

        Builder& withBorder(int pixels) override // Override to set the border size
        {
            mMenu->mBorder = pixels; // Sets the border size
            return *this;            // Return reference to the builder for method chaining
        }

        Builder& addOption(std::string o) override // Override to add a menu option
        {
            mMenu->mOpts.push_back(o); // Adds the option to the menu's list of options
            return *this;              // Return reference to the builder for method chaining
        }

        Builder& horizontal() override // Override to set the menu layout to horizontal
        {
            mMenu->mHorizontal = true; // Sets the layout to horizontal
            return *this;              // Return reference to the builder for method chaining
        }

        Builder& vertical() override // Override to set the menu layout to vertical
        {
            mMenu->mHorizontal = false; // Sets the layout to vertical
            return *this;               // Return reference to the builder for method chaining
        }

        std::unique_ptr<Menu> build() override // Override to build the final menu object
        {
            return std::move(mMenu); // Transfers ownership of the built menu to the caller
        }

    private:
        std::unique_ptr<ProductionMenu> mMenu{}; // The menu object being built
    };

    void show() const override {} // Override the show method, currently does nothing

    void select(int /* index */) const override {} // Override the select method, currently does nothing

private:
    std::string mId{};               // Menu ID
    std::string mTitle{};            // Menu title
    std::vector<std::string> mOpts{}; // List of menu options
    bool mHorizontal{false};         // Flag indicating if the menu layout is horizontal
    int mBorder{0};                  // Size of the border

    ProductionMenu(std::string id) // Private constructor initializing the menu ID
        : mId{id}
    {
    }
};
} // namespace

int main()
{
    const auto menu = ProductionMenu::ProductionBuilder{"main"} // Start building the menu
                          .withTitle("Main Menu")               // Set the title to "Main Menu"
                          .withBorder(1)                        // Set the border size to 1
                          .addOption("Option 1")                // Add "Option 1" to the menu
                          .addOption("Option 2")                // Add "Option 2" to the menu
                          .horizontal()                         // Set the menu layout to horizontal
                          .build();                             // Build the menu

    menu->show();  // Show the menu (currently does nothing)
    menu->select(1); // Select the first option (currently does nothing)

    return 0;
}
